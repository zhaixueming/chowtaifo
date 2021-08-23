#ifndef CAMPARAM_H
#define CAMPARAM_H

#include "serializer/cppserializer.hpp"
#include <fstream>
#include <iomanip>

//面阵配置
struct CamParam
{
	std::string exposuretop = "100";
	std::string gaintop = "100";
	std::string exposurein = "100";
	std::string gainin = "100";
	std::string exposureside1 = "100";
	std::string gainside1 = "100";
	std::string exposureside2 = "100";
	std::string gainside2 = "100";
	std::string exposureside3 = "100";
	std::string gainside3 = "100";
	std::string exposureside4 = "100";
	std::string gainside4 = "100";
};

DEFINE_STRUCT_SCHEMA(CamParam,
	DEFINE_STRUCT_FIELD(exposuretop, "exposuretop"),
	DEFINE_STRUCT_FIELD(gaintop, "gaintop"),
	DEFINE_STRUCT_FIELD(exposuretop, "exposurein"),
	DEFINE_STRUCT_FIELD(gaintop, "gainin"),
	DEFINE_STRUCT_FIELD(exposuretop, "exposureside1"),
	DEFINE_STRUCT_FIELD(gaintop, "gainside1"),
	DEFINE_STRUCT_FIELD(exposuretop, "exposureside2"),
	DEFINE_STRUCT_FIELD(gaintop, "gainside2"),
	DEFINE_STRUCT_FIELD(exposuretop, "exposureside3"),
	DEFINE_STRUCT_FIELD(gaintop, "gainside3"),
	DEFINE_STRUCT_FIELD(exposuretop, "exposureside4"),
	DEFINE_STRUCT_FIELD(gaintop, "gainside4")
)
#endif // !CONFIG_H