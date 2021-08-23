
#ifdef WIN32
#define  SR_3D_API extern "C" __declspec(dllexport)
#else
#define  SR_3D_API extern
#endif

/********3D��ʾ****************
****_BatchData�� ����������***********
****x_true_step��x������/mm*********
****y_true_step��y������/mm**
****x_validPoint_num��x�������ݸ���***
****y_batchPoint_num������������***
****z_scale��z��������ϵ��********
****Ho��z�������ֵ*****
****Lo��z������Сֵ*****/
SR_3D_API void SR_3D_EXE_Show(const int *_BatchData,
								double x_true_step,
								double y_true_step, 
								int x_Point_num,
								int y_batchPoint_num,
								double z_scale,
								double Ho = 100,
								double Lo = -100
								);

//���浱��ͼ��PNG�ļ�
SR_3D_API int SR_3D_EXE_SaveCurScreen(char* _file);