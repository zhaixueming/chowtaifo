/****************************************************************************
 *  @file     formuladialog.cpp
 *  @brief    配方设置界面
 *  @author
 *  @email
 *  @version
 *  @date     2020.12.21
 *  Copyright (c) 2020 SmartMore All rights reserved.
 ****************************************************************************/
#include "camsetdialog.h"
#include "configtitlebar.h"
#include "ui_camsetdialog.h"
#include "uiframe/commonuiset/messagedialog/messagedialog.h"

#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QGraphicsDropShadowEffect>

namespace vimo::display
{
	CamsetDialog::CamsetDialog(QWidget *parent) : QDialog(parent), ui(new Ui::CamsetDialog)
	{
		ui->setupUi(this);
		setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
		setWindowModality(Qt::ApplicationModal);

		initWidgets();
		getparam();
	}

	CamsetDialog::~CamsetDialog()
	{
		delete ui;
	}

	void CamsetDialog::initWidgets()
	{
		m_titleBar = new ConfigTitleBar(this);
		m_titleBar->m_pTitleContent->setText(tr("面阵相机设置"));
		ui->titlebar_layout->addWidget(m_titleBar);
		connect(m_titleBar, SIGNAL(signalClose()), this, SLOT(slotCheckAndClose()));

		info_list.push_back(ImageView::ViewInfo{ QUuid(), QString("上方相机") });
		info_list.push_back(ImageView::ViewInfo{ QUuid(), QString("内部相机") });
		info_list.push_back(ImageView::ViewInfo{ QUuid(), QString("侧面相机一") });
		info_list.push_back(ImageView::ViewInfo{ QUuid(), QString("侧面相机二") });
		info_list.push_back(ImageView::ViewInfo{ QUuid(), QString("侧面相机三") });
		info_list.push_back(ImageView::ViewInfo{ QUuid(), QString("侧面相机四") });
		m_view_frame = new SixViewsWidget(info_list);
		ui->gridLayoutCam->addWidget(m_view_frame);
	}

	void CamsetDialog::slotCheckAndClose()
	{
		accept();
	}

	//保存按钮
	void CamsetDialog::on_ButtonSave_clicked()
	{
		setparam();
	}

	//取消按钮
	void CamsetDialog::on_ButtonCancel_clicked()
	{
		accept();
	}

	//获取参数
	void CamsetDialog::getparam()
	{
		std::string paramDir = QCoreApplication::applicationDirPath().toStdString() + "/CamParam.json";
		_camparam = GetCamParamFromFile(paramDir);

		ui->lineEditEtop->setText(QString::fromStdString(_camparam.exposuretop));
		ui->lineEditGtop->setText(QString::fromStdString(_camparam.gaintop));
		ui->lineEditEin->setText(QString::fromStdString(_camparam.exposurein));
		ui->lineEditGin->setText(QString::fromStdString(_camparam.gainin));
		ui->lineEditEside1->setText(QString::fromStdString(_camparam.exposureside1));
		ui->lineEditGside1->setText(QString::fromStdString(_camparam.gainside1));
		ui->lineEditEside2->setText(QString::fromStdString(_camparam.exposureside2));
		ui->lineEditGside2->setText(QString::fromStdString(_camparam.gainside2));
		ui->lineEditEside3->setText(QString::fromStdString(_camparam.exposureside3));
		ui->lineEditGside3->setText(QString::fromStdString(_camparam.gainside3));
		ui->lineEditEside4->setText(QString::fromStdString(_camparam.exposureside4));
		ui->lineEditGside4->setText(QString::fromStdString(_camparam.gainside4));
	}

	//设置参数
	void CamsetDialog::setparam()
	{
		_camparam.exposuretop = ui->lineEditEtop->text().toStdString();
		_camparam.gaintop = ui->lineEditGtop->text().toStdString();
		_camparam.exposurein = ui->lineEditEin->text().toStdString();
		_camparam.gainin = ui->lineEditGin->text().toStdString();
		_camparam.exposureside1 = ui->lineEditEside1->text().toStdString();
		_camparam.gainside1 = ui->lineEditGside1->text().toStdString();
		_camparam.exposureside2 = ui->lineEditEside2->text().toStdString();
		_camparam.gainside2 = ui->lineEditGside2->text().toStdString();
		_camparam.exposureside3 = ui->lineEditEside3->text().toStdString();
		_camparam.gainside3 = ui->lineEditGside3->text().toStdString();
		_camparam.exposureside4 = ui->lineEditEside4->text().toStdString();
		_camparam.gainside4 = ui->lineEditGside4->text().toStdString();

		Savecamparam();
	}

	//读取文件
	CamParam CamsetDialog::GetCamParamFromFile(std::string path)
	{
		CamParam param;
		std::ifstream fi(path.c_str());
		if (fi.good())
		{
			nlohmann::json j;
			fi >> j;
			param = j.get<CamParam>();
		}
		else
		{
			MessageDialog message_box_dialog(DialogType::errorDialog, tr("Get cam Param File faild!"));
			message_box_dialog.exec();
		}
		return param;
	}

	void CamsetDialog::Savecamparam()
	{
		std::string sysparamDir = QCoreApplication::applicationDirPath().toStdString() + "/StereoscopicParam.json";
		std::ofstream fo(sysparamDir.c_str());
		nlohmann::json j = _camparam;
		fo << std::setw(4) << j << std::endl;
		fo.flush();
	}

	//单张取图
	void CamsetDialog::on_Buttononetop_clicked()
	{

	}
	void CamsetDialog::on_Buttononein_clicked()
	{

	}
	void CamsetDialog::on_Buttononeside1_clicked()
	{

	}
	void CamsetDialog::on_Buttononeside2_clicked()
	{

	}
	void CamsetDialog::on_Buttononeside3_clicked()
	{

	}
	void CamsetDialog::on_Buttononeside4_clicked()
	{

	}
	//实时取图
	void CamsetDialog::on_Buttonlivetop_clicked()
	{

	}
	void CamsetDialog::on_Buttonlivein_clicked()
	{

	}
	void CamsetDialog::on_Buttonliveside1_clicked()
	{

	}
	void CamsetDialog::on_Buttonliveside2_clicked()
	{

	}
	void CamsetDialog::on_Buttonliveside3_clicked()
	{

	}
	void CamsetDialog::on_Buttonliveside4_clicked()
	{

	}
	//测试参数
	void CamsetDialog::on_Buttontesttop_clicked()
	{
		bool btexte, btextg;
		float exp = ui->lineEditEtop->text().toFloat(&btexte);
		float gain = ui->lineEditGtop->text().toFloat(&btextg);
		if (btexte && btextg)
		{
			emit sendcamtopesignal(exp);
			emit sendcamtopgsignal(gain);
		}
	}
	void CamsetDialog::on_Buttontestin_clicked()
	{
		bool btexte, btextg;
		float exp = ui->lineEditEin->text().toFloat(&btexte);
		float gain = ui->lineEditGin->text().toFloat(&btextg);
		if (btexte && btextg)
		{
			emit sendcaminesignal(exp);
			emit sendcamingsignal(gain);
		}
	}
	void CamsetDialog::on_Buttontestside1_clicked()
	{
		bool btexte, btextg;
		float exp = ui->lineEditEside1->text().toFloat(&btexte);
		float gain = ui->lineEditGside1->text().toFloat(&btextg);
		if (btexte && btextg)
		{
			emit sendcamside1esignal(exp);
			emit sendcamside1gsignal(gain);
		}
	}
	void CamsetDialog::on_Buttontestside2_clicked()
	{
		bool btexte, btextg;
		float exp = ui->lineEditEside2->text().toFloat(&btexte);
		float gain = ui->lineEditGside2->text().toFloat(&btextg);
		if (btexte && btextg)
		{
			emit sendcamside2esignal(exp);
			emit sendcamside2gsignal(gain);
		}
	}
	void CamsetDialog::on_Buttontestside3_clicked()
	{
		bool btexte, btextg;
		float exp = ui->lineEditEside3->text().toFloat(&btexte);
		float gain = ui->lineEditGside3->text().toFloat(&btextg);
		if (btexte && btextg)
		{
			emit sendcamside1esignal(exp);
			emit sendcamside1gsignal(gain);
		}
	}
	void CamsetDialog::on_Buttontestside4_clicked()
	{
		bool btexte, btextg;
		float exp = ui->lineEditEside4->text().toFloat(&btexte);
		float gain = ui->lineEditGside4->text().toFloat(&btextg);
		if (btexte && btextg)
		{
			emit sendcamside4esignal(exp);
			emit sendcamside4gsignal(gain);
		}
	}
	//存图
	void CamsetDialog::on_Buttonsaveimagetop_clicked()
	{
		QString Folder = "F:/Image/";
		QDir ImageFolder(Folder);
		if (!ImageFolder.exists())
		{
			ImageFolder.mkdir(Folder);
		}

		QString file = "L_.bmp";
		QString StringFile = QFileDialog::getSaveFileName(this, tr("Save Image"), Folder + file, tr("*.bmp;; *.png;; *.jpg;; *.tif;; *.GIF"));
		if (StringFile == NULL)
			return;

		cv::Mat ResizeImage;
		cv::imwrite(StringFile.toStdString(), ResizeImage);
		MessageDialog message_box_dialog(DialogType::infoDialog, tr("存图成功!"));
		message_box_dialog.exec();
	}
	void CamsetDialog::on_Buttonsaveimagein_clicked()
	{

	}
	void CamsetDialog::on_Buttonsaveimageside1_clicked()
	{

	}
	void CamsetDialog::on_Buttonsaveimageside2_clicked()
	{

	}
	void CamsetDialog::on_Buttonsaveimageside3_clicked()
	{

	}
	void CamsetDialog::on_Buttonsaveimageside4_clicked()
	{

	}
}  // namespace vtk::display
