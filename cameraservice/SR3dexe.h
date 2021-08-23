
#ifdef WIN32
#define  SR_3D_API extern "C" __declspec(dllexport)
#else
#define  SR_3D_API extern
#endif

/********3D显示****************
****_BatchData： 批处理数据***********
****x_true_step：x方向间矩/mm*********
****y_true_step：y方向间距/mm**
****x_validPoint_num：x方向数据个数***
****y_batchPoint_num：批处理行数***
****z_scale：z方向缩放系数********
****Ho：z方向最大值*****
****Lo：z方向最小值*****/
SR_3D_API void SR_3D_EXE_Show(const int *_BatchData,
								double x_true_step,
								double y_true_step, 
								int x_Point_num,
								int y_batchPoint_num,
								double z_scale,
								double Ho = 100,
								double Lo = -100
								);

//保存当视图到PNG文件
SR_3D_API int SR_3D_EXE_SaveCurScreen(char* _file);