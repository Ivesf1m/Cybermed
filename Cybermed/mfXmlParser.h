#pragma once

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "mfMacros.h"
#include <string>
#include <vector>

using std::string;
using std::vector;

namespace mf
{
	class mfXmlParser
	{
	public:

		/** Constructor */
		mfXmlParser()
			: vdim(0), cdim(0)
		{

		};

		~mfXmlParser() {

		};

		bool read(string& filename)
		{
			FILE* pf;
			char *buffer, *p;
			char sep[] = " <>\n";
			int status = 0;
			int i;
			int vtemp = 0;
			int ctemp = 0;
			long lsize;
			bool ler;

			if (!(pf = fopen(filename.c_str(), "rt")))
				return false;

			fseek(pf, 0, SEEK_END);
			lsize = ftell(pf);
			rewind(pf);
			assert(lsize > 0);
			buffer = new char[lsize];

			fread(buffer, 1, lsize, pf);
			fclose(pf);

			// status
			// 0 : no command
			// 1 : TES_XML
			// 2 : database
			// 3 : tables
			// 4 : vertices
			// 5 : cells
			p = strtok(buffer, sep);
			while (p != NULL) {
				ler = true;

				if (status == 0) {
					if (strcmp(p, "MF_XML") == 0)
						status = 1;
				}
				else if (status == 1) {
					if (strcmp(p, "database") == 0)
						status = 2;
					else if (strcmp(p, "/MF_XML") == 0)
						status = 0;
				}
				else if (status == 2) {
					if (strcmp(p, "name") == 0)
						db = string(strtok(NULL, sep));
					else if (strcmp(p, "host") == 0)
						host = string(strtok(NULL, sep));
					else if (strcmp(p, "port") == 0)
						port = string(strtok(NULL, sep));
					else if (strcmp(p, "user") == 0)
						user = string(strtok(NULL, sep));
					else if (strcmp(p, "password") == 0)
						pass = string(strtok(NULL, sep));
					else if (strcmp(p, "tables") == 0)
						status = 3;
					else if (strcmp(p, "/database") == 0)
						status = 1;
				}
				else if (status == 3) {
					if (strcmp(p, "vertices") == 0)
						status = 4;
					else if (strcmp(p, "cells") == 0)
						status = 5;
					else if (strcmp(p, "/tables") == 0)
						status = 2;
				}
				else if (status == 4) {
					if (strcmp(p, "name") == 0)
						vtable = string(strtok(NULL, sep));
					else if (strcmp(p, "id") == 0)
						vid = string(strtok(NULL, sep));
					else if (strcmp(p, "dimension") == 0) {
						vdim = atoi(strtok(NULL, sep));
						if (vdim > 0) {
							coords.resize(vdim);
						}
					}
					else if (strcmp(p, "coord") == 0) {
						assert(vtemp < vdim);
						coords[vtemp++] = string(strtok(NULL, sep));
					}
					else if (strcmp(p, "/vertices") == 0)
						status = 3;
				}
				else if (status == 5) {
					if (strcmp(p, "name") == 0)
						ctable = string(strtok(NULL, sep));
					else if (strcmp(p, "id") == 0)
						cid = string(strtok(NULL, sep));
					else if (strcmp(p, "dimension") == 0) {
						cdim = atoi(strtok(NULL, sep));
						if (cdim > 0) {
							vertices.resize(cdim);
						}
					}
					else if (strcmp(p, "vertex") == 0) {
						assert(ctemp < cdim);
						vertices[ctemp++] = string(strtok(NULL, sep));
					}
					else if (strcmp(p, "/cells") == 0)
						status = 3;
				}

				if (read)
					p = strtok(NULL, sep);
			}

			delete[] buffer;

			if ((status == 0) && (vdim > 0) && (cdim > 0))
				return true;
			else
				return false;
		};

		string& getHost() { return host; };
		string& getPort() { return port; };
		string& getUser() { return user; };
		string& getPassword() { return pass; };
		string& getDatabase() { return db; };
		string& getVerticesTable() { return vtable; };
		string& getCellsTable() { return ctable; };
		string& getVerticesIdField() { return vid; };
		int getVerticesDimension() { return vdim; };
		string& getVerticesField(int dim) { return coords[dim]; };
		string& getCellsIdField() { return cid; };
		int getCellsDimension() { return cdim; };
		string& getCellsField(int dim) { return vertices[dim]; };

	private:
		string host;
		string port;
		string user;
		string pass;
		string db;
		string vtable;
		string ctable;
		string vid;
		string cid;
		vector<string> vertices;
		vector<string> coords;
		int vdim;
		int cdim;
	};
	
}