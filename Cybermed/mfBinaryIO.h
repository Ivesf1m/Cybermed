#pragma once

#include <fstream>
#include "mfMacros.h"

using std::ifstream;
using std::ofstream;

namespace mf
{
#define SPACE typename mfBinaryIO<Traits>::space
#define IDS typename mfBinaryIO<Traits>::ids

	template<class Traits>
	class mfBinaryIO
	{
		public:
			typedef typename Traits::space space;
			typedef typename Traits::ids ids;

			mfBinaryIO();

			~mfBinaryIO();

			void idsStore(ofstream& fp, ids num);
			ids idsLoad(ifstream& fp);

			void spaceStore(ofstream& fp, space num);
			space spaceLoad(ifstream& fp);
	};

	template<class Traits>
	mfBinaryIO<Traits>::mfBinaryIO()
	{

	}

	template<class Traits>
	mfBinaryIO<Traits>::~mfBinaryIO()
	{

	}

	template<class Traits>
	void mfBinaryIO<Traits>::idsStore(ofstream& fp, ids num)
	{
		unsigned char flag;
		unsigned char f = 0;

		if (num < 0) {
			f = 32;
			num *= -1;
		}

		if (num < 8192) {
			flag = f | (unsigned char)(num >> 8);
			fp.write((char*)&flag, sizeof(unsigned char));
			flag = (unsigned char)num;
			fp.write((char*)&flag, sizeof(unsigned char));
		}
		else if (num < 2097152) {
			flag = f | 64 | (unsigned char)(num >> 16);
			fp.write((char*)&flag, sizeof(unsigned char));
			flag = (unsigned char)(num >> 8);
			fp.write((char*)&flag, sizeof(unsigned char));
			flag = (unsigned char)num;
			fp.write((char*)&flag, sizeof(unsigned char));
		}
		else if (num < 536870912) {
			flag = f | 128 | (unsigned char)(num >> 24);
			fp.write((char*)&flag, sizeof(unsigned char));
			flag = (unsigned char)(num >> 16);
			fp.write((char*)&flag, sizeof(unsigned char));
			flag = (unsigned char)(num >> 8);
			fp.write((char*)&flag, sizeof(unsigned char));
			flag = (unsigned char)num;
			fp.write((char*)&flag, sizeof(unsigned char));
		}
		else {
			flag = f | 192;
			fp.write((char*)&flag, sizeof(unsigned char));
			flag = (unsigned char)(num >> 24);
			fp.write((char*)&flag, sizeof(unsigned char));
			flag = (unsigned char)(num >> 16);
			fp.write((char*)&flag, sizeof(unsigned char));
			flag = (unsigned char)(num >> 8);
			fp.write((char*)&flag, sizeof(unsigned char));
			flag = (unsigned char)num;
			fp.write((char*)&flag, sizeof(unsigned char));
		}
	}

	template<class Traits>
	IDS mfBinaryIO<Traits>::idsLoad(ifstream& fp)
	{
		ids num;
		unsigned char flag, tam;
		bool s;

		fp.read((char*)&flag, sizeof(unsigned char));
		tam = flag << 2;
		s = tam >= 128;
		tam = flag >> 6;

		if (tam == 0) {
			num = (flag & 31);
			fp.read((char*)&flag, sizeof(unsigned char));
			num = (num << 8) | flag;
		}
		else if (tam == 1) {
			num = (flag & 31);
			fp.read((char*)&flag, sizeof(unsigned char));
			num = (num << 8) | flag;
			fp.read((char*)&flag, sizeof(unsigned char));
			num = (num << 8) | flag;
		}
		else if (tam == 2) {
			num = (flag & 31);
			fp.read((char*)&flag, sizeof(unsigned char));
			num = (num << 8) | flag;
			fp.read((char*)&flag, sizeof(unsigned char));
			num = (num << 8) | flag;
			fp.read((char*)&flag, sizeof(unsigned char));
			num = (num << 8) | flag;
		}
		else {
			fp.read((char*)&flag, sizeof(unsigned char));
			num = flag;
			fp.read((char*)&flag, sizeof(unsigned char));
			num = (num << 8) | flag;
			fp.read((char*)&flag, sizeof(unsigned char));
			num = (num << 8) | flag;
			fp.read((char*)&flag, sizeof(unsigned char));
			num = (num << 8) | flag;
		}

		if (s)
			num *= -1;
		return num;
	}

	template<class Traits>
	void mfBinaryIO<Traits>::spaceStore(ofstream& fp, space num) 
	{
		ids approx;
		unsigned char flag = 0;

		if (num < 0) {
			flag = 1;
			num *= -1.0;
		}

		while ((num < (space) 214748364.7) && ((flag >> 1) < 127)) {
			flag += 2;
			num *= 10.0;
		}

		approx = (ids)num;

		fp.write((char*)&flag, sizeof(unsigned char));
		flag = (unsigned char)(approx >> 24);
		fp.write((char*)&flag, sizeof(unsigned char));
		flag = (unsigned char)(approx >> 16);
		fp.write((char*)&flag, sizeof(unsigned char));
		flag = (unsigned char)(approx >> 8);
		fp.write((char*)&flag, sizeof(unsigned char));
		flag = (unsigned char) approx;
		fp.write((char*)&flag, sizeof(unsigned char));
	}

	template<class Traits>
	SPACE mfBinaryIO<Traits>::spaceLoad(ifstream& fp)
	{
		space num;
		ids approx;
		unsigned char flag;
		unsigned char esp;
		bool s;

		fp.read((char*)&esp, sizeof(unsigned char));
		fp.read((char*)&flag, sizeof(unsigned char));
		approx = flag;
		fp.read((char*)&flag, sizeof(unsigned char));
		approx = (approx << 8) | flag;
		fp.read((char*)&flag, sizeof(unsigned char));
		approx = (approx << 8) | flag;
		fp.read((char*)&flag, sizeof(unsigned char));
		approx = (approx << 8) | flag;

		s = ((esp % 2) == 1);
		esp = esp >> 1;
		num = (space)approx;

		while (esp > 0) {
			num /= 10.0;
			esp--;
		}

		if (s)
			num *= -1.0;

		return num;
	}

#undef SPACE
#undef IDS
}