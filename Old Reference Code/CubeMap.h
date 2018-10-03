#pragma once


namespace gp
{
	class CubeMap
	{
	public:

		CubeMap(unsigned int id);
		CubeMap(CubeMap &&c);
		~CubeMap();

	private:

		friend class Shader;
		friend class Resources;
		unsigned int _id;

	private:

		CubeMap(const CubeMap &c) = delete;
		CubeMap& operator= (const CubeMap &c) = delete;

	};
}
