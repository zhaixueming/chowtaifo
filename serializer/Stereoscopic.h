#ifndef STEREOSCOPIC_H
#define STEREOSCOPIC_H

#include "serializer/cppserializer.hpp"
#include <fstream>
#include <iomanip>

//3D配置
struct StereoscopicParam
{
	std::string exposure = "100";
	std::string refined = "100";
	std::string frequency = "100";
	std::string batch = "100";
};

DEFINE_STRUCT_SCHEMA(StereoscopicParam,
	DEFINE_STRUCT_FIELD(exposure, "exposure"),
	DEFINE_STRUCT_FIELD(refined, "refined"),
	DEFINE_STRUCT_FIELD(frequency, "frequency"),
	DEFINE_STRUCT_FIELD(batch, "batch")
)
#endif // !CONFIG_H