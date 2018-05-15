#pragma once

#include "mfMesh.h"
#include "mfTReader.h"
#include <thread>

using std::thread;

namespace
{
	template<class Traits>
	class mfPgReader : public mfTReader<Traits>
	{
	public:
		typedef typename Traits::space space;
		typedef typename Traits::ids ids;
		typedef typename Traits::sVertex sVertex;
		typedef typename Traits::sCell sCell;
		typedef typename Traits::sMesh sMesh;

		/** Constructor */
		mfPgReader(int vthreads = 2, int cthreads = 2);

		/** Destructor */
		virtual ~mfPgReader();

		bool read(sMesh& mesh, string& xml);

	private:
		typedef typename mfTReader<Traits>::mfTReaderData mfTReaderData;

		static void* threadPoint(mfTReaderData* obj);
		static void* threadCell(mfTReaderData* obj);
	};

	template<class Traits>
	mfPgReader<Traits>::mfPgReader(int vthreads, int cthreads)
		: mfTReader<Traits>(vthreads, cthreads)
	{

	}

	template<class Traits>
	mfPgReader<Traits>::~mfPgReader()
	{

	}

	template<class Traits>
	bool mfPgReader<Traits>::read(sMesh& mesh, string& xml)
	{
		PGconn* conn;
		PGresult* res;
		char conn_str[MAX_BUFFER];
		char buffer[MAX_BUFFER];
		int i, nv, nc;

		mfXmlParser config;
		config.read(xml);
		sprintf(conn_str, "host=%s port=%s dbname=%s user=%s password=%s",
			config.getHost(), config.getPort(), config.getDatabase(),
			config.getUser(), config.getPassword());
		conn = PQconnectdb(conn_str);

		if (PQstatus(conn) != CONNECTION_OK) {
			fprintf(stderr, "Connection to database failed: %s",
				PQerrorMessage(conn));
			PQfinish(conn);
			return false;
		}

		cout << "Postgres conectado." << endl;
		sprintf(buffer, "SELECT count(*) as num FROM %s",
			config.getVerticesTable());
		res = PQexec(conn, buffer);

		if (PQresultStatus(res) != PGRES_TUPLES_OK) {
			fprintf(stderr, "SELECT failed: %s", PQerrorMessage(conn));
			PQclear(res);
			PQfinish(conn);
			return false;
		}

		nv = atoi(PQgetvalue(res, 0, 0));
		assert(nv > 0);
		PQclear(res);

		thread tid[vThreads];
		mfTReaderData pdobj[vThreads];
		ids qtd = nv / vThreads;
		ids start = 0;

		for (i = 0; i < vThreads; ++i) {
			pdobj[i].id = i;
			pdobj[i].str = string(conn_str);
			pdobj[i].start = start;
			pdobj[i].end = start + qtd;
			if (pdobj[i].end >= nv)
				pdobj[i].end = nv - 1;
			pdobj[i].mesh = &mesh;
			pdobj[i].config = &config;

			start += qtd + 1;
			tid[i] = thread(threadPoint, pdobj + i);
		}

		for (i = 0; i < vThreads; ++i)
			tid[i].join();

		mesh.setNumberOfVertices(nv);

		thread ctid[cThreads];
		mfTReaderData cpdobj[cThreads];

		sprintf(buffer, "SELECT count(*) as num FROM %s", config.getCellsTable());
		res = PQexec(conn, buffer);
		if (PQresultStatus(res) != PGRES_TUPLES_OK) {
			fprintf(stderr, "SELECT failed: %s", PQerrorMessage(conn));
			PQclear(res);
			PQfinish(conn);
			return false;
		}

		nc = atoi(PQgetvalue(res, 0, 0));
		assert(nc > 0);
		PQclear(res);

		qtd = nc / cThreads;
		start = 0;

		for (i = 0; i < cThreads; ++i) {
			pdobj[i].id = i;
			pdobj[i].str = string(conn_str);
			pdobj[i].start = start;
			pdobj[i].end = start + qtd;
			if (pdobj[i].end >= nc)
				pdobj[i].end = nc - 1;
			pdobj[i].mesh = &mesh;
			pdobj[i].config = &config;

			start += qtd + 1;
			tid[i] = thread(threadCell, pdobj + i);
		}

		for (i = 0; i < vThreads; ++i)
			tid[i].join();

		mesh.setNumberOfCells(nc);

		PQfinish(conn);
		return true;
	}

	template<class Traits>
	void* mfPgReader<Traits>::threadPoint(mfTReaderData* obj)
	{
		assert(obj);
		PGconn* conn;
		PGresult* res;
		char buffer[MAX_BUFFER];
		ids start, end, qtd;

		conn = PQconnectdb(obj->str);

		if (PQstatus(conn) != CONNECTION_OK) {
			fprintf(stderr, "Connection to database failed: %s",
				PQerrorMessage(conn));
			PQfinish(conn);
			return false;
		}

		qtd = MF_TREADER_QTDSQL;
		assert(qtd > 0);
		start = obj->start;
		while (start <= obj->end) {
			end = start + qtd;
			if (end > obj->end)
				end = obj->end;
			sprintf(buffer, "SELECT * FROM %s WHERE ((%s >= %d) and (%s <= %d))",
				obj->config->getVerticesTable(), obj->config->getVerticesIdField(),
				start, obj->config->getVerticesIdField(), end);
			res = PQexec(conn, buffer);

			if (PQresultStatus(res) != PGRES_TUPLES_OK) {
				fprintf(stderr, "SELECT failed: %s", PQerrorMessage(conn));
				PQclear(res);
				PQfinish(conn);
				return false;
			}
			else {
				int idfnum, xfnum, yfnum, zfnum, id, num, i;
				space coords[sVertex::getDimension()];

				idfnum = PQfnumber(res, obj->config->getVerticesIdField());
				xfnum = PQfnumber(res, obj->config->getVerticesField(0));
				yfnum = PQfnumber(res, obj->config->getVerticesField(1));
				if (sVertex::getDimension() == 3)
					zfnum = PQfnumber(res, obj->config->getVerticesField(2));
				num = PQntuples(res);
				assert(num > 0);
				for (i = 0; i < num; ++i) {
					id = atoi(PQgetvalue(res, i, idfnum));
					coords[0] = atmf(PQgetvalue(res, i, xfnum));
					coords[1] = atmf(PQgetvalue(res, i, yfnum));
					if (sVertex::getDimension() == 3)
						coords[2] = atmf(PQgetvalue(res, i, zfnum));
					obj->mesh->addVertex(id, coords);
				}
			}
			start += qtd + 1;
			PQclear(res);
		}

		PQfinish(conn);
		return NULL;
	}

	template<class Traits>
	void* mfPgReader<Traits>::threadCell(mfTReaderData* obj)
	{
		assert(obj);
		PGconn* conn;
		PGresult* res;
		char buffer[MAX_BUFFER];
		ids start, end, qtd;

		conn = PQconnectdb(obj->str);

		if (PQstatus(conn) != CONNECTION_OK) {
			fprintf(stderr, "Connection to database failed: %s",
				PQerrorMessage(conn));
			PQfinish(conn);
			return false;
		}

		int idfnum, v1fnum, v2fnum, v3fnum, v4fnum;
		ids id, v[sCell::getDimension()], num, i;

		qtd = MF_TREADER_QTDSQL;
		assert(qtd > 0);
		start = obj->start;
		while (start <= obj->end) {
			end = start + qtd;
			if (end > obj->end)
				end = obj->end;
			sprintf(buffer, "SELECT * FROM %s WHERE ((%s >= %d) and (%s <= %d))",
				obj->config->getCellsTable(), obj->config->getCellsIdField(),
				start, obj->config->getCellsIdField(), end);
			res = PQexec(conn, buffer);

			if (PQresultStatus(res) != PGRES_TUPLES_OK) {
				fprintf(stderr, "SELECT failed: %s", PQerrorMessage(conn));
				PQclear(res);
				PQfinish(conn);
				return false;
			}
			else {
				num = PQntuples(res);
				assert(num > 0);
				idfnum = PQfnumber(res, obj->config->getCellsIdField());
				v1fnum = PQfnumber(res, obj->config->getCellsField(0));
				v2fnum = PQfnumber(res, obj->config->getCellsField(1));
				v3fnum = PQfnumber(res, obj->config->getCellsField(2));
				if(sCell::getDimension() == 3)
					v4fnum = PQfnumber(res, obj->config->getCellsField(3));

				for (i = 0; i < num; ++i) {
					id = atoi(PQgetvalue(res, i, idfnum));
					v[0] = atoi(PQgetvalue(res, i, v1fnum));
					v[1] = atoi(PQgetvalue(res, i, v2fnum));
					v[2] = atoi(PQgetvalue(res, i, v3fnum));
					if (sCell::getDimension() == 3)
						v[3] = atoi(PQgetvalue(res, i, v4fnum));
					assert(v[0] >= 0);
					assert(v[1] >= 0);
					assert(v[2] >= 0);
					if(sCell::getDimension() == 3)
						assert(v[3] >= 0);
					obj->mesh->addCell(id, v);
				}
			}

			start += qtd + 1;
			PQclear(res);
		}

		PQfinish(conn);
		return NULL;
	}
}
