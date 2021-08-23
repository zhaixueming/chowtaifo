#ifndef CONFIG_H
#define CONFIG_H

#include "serializer/cppserializer.hpp"
#include <fstream>
#include <iomanip>

//首页配置
struct SystemParam
{
	//plc
	std::string plc_ipadd = "192.168.250.1";
	
	std::string plc_rotate = "W0.0";
	std::string plc_restoration = "W0.1";
	std::string plc_allow = "W0.2";
	std::string plc_state = "W0.3";
	std::string plc_Allowed = "W0.3";
	std::string plc_result = "W0.3";
	std::string plc_accomplish = "W0.3";

	std::string stereo_ip = "W0.3";
	std::string top_number = "W0.3";
	std::string in_number = "W0.3";
	std::string side1_number = "W0.3";
	std::string side2_number = "W0.3";
	std::string side3_number = "W0.3";
	std::string side4_number = "W0.3";
	
	std::string stereo_save = "D:/1.ccf";
	std::string top_save = "D:/2.ccf";
	std::string in_save = "D:/3.ccf";
	std::string side1_save = "D:/4.ccf";
	std::string side2_save = "E:/";
	std::string side3_save = "E:/";
	std::string side4_save = "E:/";
};

DEFINE_STRUCT_SCHEMA(SystemParam,
	DEFINE_STRUCT_FIELD(plc_ipadd, "plc_ipadd"),
	
	DEFINE_STRUCT_FIELD(plc_rotate, "plc_rotate"),
	DEFINE_STRUCT_FIELD(plc_restoration, "plc_restoration"),
	DEFINE_STRUCT_FIELD(plc_allow, "plc_allow"),
	DEFINE_STRUCT_FIELD(plc_state, "plc_state"),
	DEFINE_STRUCT_FIELD(plc_Allowed, "plc_Allowed"),
	DEFINE_STRUCT_FIELD(plc_result, "plc_result"),
	DEFINE_STRUCT_FIELD(plc_accomplish, "plc_accomplish"),

	DEFINE_STRUCT_FIELD(stereo_ip, "stereo_ip"),
	DEFINE_STRUCT_FIELD(top_number, "top_number"),
	DEFINE_STRUCT_FIELD(in_number, "in_number"),
	DEFINE_STRUCT_FIELD(side1_number, "side1_number"),
	DEFINE_STRUCT_FIELD(side2_number, "side2_number"),
	DEFINE_STRUCT_FIELD(side3_number, "side3_number"),
	DEFINE_STRUCT_FIELD(side4_number, "side4_number"),
	
	DEFINE_STRUCT_FIELD(stereo_save, "stereo_save"),
	DEFINE_STRUCT_FIELD(top_save, "top_save"),
	DEFINE_STRUCT_FIELD(in_save, "in_save"),
	DEFINE_STRUCT_FIELD(side1_save, "side1_save"),
	DEFINE_STRUCT_FIELD(side2_save, "side2_save"),
	DEFINE_STRUCT_FIELD(side3_save, "side3_save"),
	DEFINE_STRUCT_FIELD(side4_save, "side4_save")
)
#endif // !CONFIG_H