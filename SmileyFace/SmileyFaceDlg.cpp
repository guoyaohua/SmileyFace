
// SmileyFaceDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SmileyFace.h"
#include "SmileyFaceDlg.h"
#include "afxdialogex.h"
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <iostream>
//#include <stdio.h>
#include<windows.h>
//#include "CimageAndMat.h"  
using namespace std;
using namespace cv;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



 bool isFirst;
/////////////////////////////
//人脸识别部分
CvVideoWriter* writer = 0;
bool CloseThread = true;
bool doRecognize = false;//默认为不识别，仅监测人脸。
int takeSample = 0;//用于判断是否采集样本。 
                        //takeSample = 0时训练，不采样、1时不训练，不采样、2时训练、采样。
IplImage* pFrame = NULL;  
Mat cam_frame;
CvCapture * pCapture;
//static CvCapture* pCapture;
CEdit* pEdtConfidence;
bool trainErro = false;
int sampleNum = -1;

//static bool takeSample = false;
//CvCapture* capture;
//CRect rect;
//CDC *pDC;
//HDC hDC;
//CWnd *pwnd;
//CvVideoWriter* writer = 0;
//IplImage *resizeRes;//存放检测到的人脸
//IplImage* faceGray;//存放检测到的人脸 灰度图像
//bool bool_cameOpen = false;//全局变量 标志摄像头是否打开
bool bool_picNum = false;//全局变量 标志训练图片是否为空
bool bool_detec_reco = false;//全局变量 
double dConfidence = 0;//置信度
int predictedLabel = 100000;

//CvMemStorage* storage = 0;
//CvHaarClassifierCascade* cascade = 0;
//CvHaarClassifierCascade* nested_cascade = 0;
//int use_nested_cascade = 0;
//const char* cascade_name =
 //   "../data/haarcascades/haarcascade_frontalface_alt.xml";
//const char* nested_cascade_name =
 //   "../data/haarcascade_eye_tree_eyeglasses.xml";
double scale = 1;
int num_components = 9;
double facethreshold = 9.0;
//cv::Ptr<cv::FaceRecognizer> model = cv::createFisherFaceRecognizer();
cv::Ptr<cv::FaceRecognizer> model = cv::createLBPHFaceRecognizer();//LBP的这个方法在单个人脸验证方面效果最好
//cv::Ptr<cv::FaceRecognizer> model = cv::createEigenFaceRecognizer();
//vector<Mat> images;
//vector<int> labels;

//IplImage *frame, *frame_copy = 0;
//IplImage *image = 0;
const char* scale_opt = "--scale="; // 分类器选项指示符号 
int scale_opt_len = (int)strlen(scale_opt);
const char* cascade_opt = "--cascade=";
int cascade_opt_len = (int)strlen(cascade_opt);
const char* nested_cascade_opt = "--nested-cascade";
int nested_cascade_opt_len = (int)strlen(nested_cascade_opt);
int i;
const char* input_name = 0;

// CAboutDlg dialog used for App About
CString strConfidence = L"70";
//CEdit* pEdtConfidence;
CString strTip = L"";
//CEdit* pTip;





//////////////////////////////



//声明变量
bool doShowFace = false;
CImage static image;  //声明全局变量用来保存图片；
CImage static imageShow;  //声明全局变量用来保存识别后的图片；
CString path;//图片地址全局变量
CRect rect_face;//face图片大小
CRect rect_image;//image图片大小
CRect rect;//图片显示区域大小
CRect Facerect;//Face区域大小
CDC *pDc;//pictrue控件的Dc
CDC *pDc_face;//face控件的Dc
CStatic* pStatic;//清除画布用的
/** Function Headers */
void detectAndDisplay( Mat& frame,CDC *pDc);
void detectAndDisplay2( Mat& frame ,int type);
void ShowFace(CDC *pDc,Mat faceROI);//显示人脸函数
void ShowPath(CString path);
//void detect_and_draw(IplImage* image);
String face_cascade_name = "haarcascade_frontalface_alt.xml";
//String eyes_cascade_name = "haarcascade_eye_tree_eyeglasses.xml";
String eyes_cascade_name = "haarcascade_eye.xml";
String mouth_cascade_name = "haarcascade_mcs_mouth.xml";
CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;
CascadeClassifier mouth_cascade;
	/*MatToCImage
	*简介：
	*	OpenCV的Mat转ATL/MFC的CImage，仅支持单通道灰度或三通道彩色
	*参数：
	*	mat：OpenCV的Mat
	*	cimage：ATL/MFC的CImage
	*/
	void MatToCImage( Mat& mat, CImage& cimage);


	/*CImageToMat
	*简介：
	*	ATL/MFC的CImage转OpenCV的Mat，仅支持单通道灰度或三通道彩色
	*参数：
	*	cimage：ATL/MFC的CImage
	*	mat：OpenCV的Mat
	*/
	void CImageToMat(CImage& cimage, Mat& mat);





// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}
/*
CSmileyFaceDlg::~CSmileyFaceDlg()

{

   AfxMessageBox(L"析构函数");  //弹出对话框

}
*/

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSmileyFaceDlg 对话框




CSmileyFaceDlg::CSmileyFaceDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSmileyFaceDlg::IDD, pParent)
	, m_Result_Text(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSmileyFaceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, ID_IMAGEMAP, m_Picture);
	DDX_Control(pDX, IDC_FACE_IMAGE, m_Face_Image);
	//	DDX_Control(pDX, IDC_RESULT_TEXT, m_Result_Text);
	DDX_Control(pDX, IDC_CHECK1, m_CheckBox);
	DDX_Control(pDX, IDC_EDIT1, pEdtConfidence);
}

BEGIN_MESSAGE_MAP(CSmileyFaceDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OPEN_PIC_BUTTON, &CSmileyFaceDlg::OnBnClickedOpenPicButton)
	ON_BN_CLICKED(IDC_START_BUTTON, &CSmileyFaceDlg::OnBnClickedStartButton)
	//ON_BN_CLICKED(IDC_EXIT_BUTTON, &CSmileyFaceDlg::OnBnClickedExitButton)
//	ON_BN_CLICKED(IDC_BUTTON1, &CSmileyFaceDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_CAM_BUTTON, &CSmileyFaceDlg::OnBnClickedCamButton)
	ON_BN_CLICKED(IDC_CHECK1, &CSmileyFaceDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_RECOGNIZE_BUTTON, &CSmileyFaceDlg::OnBnClickedRecognizeButton)
	ON_BN_CLICKED(IDC_SAMPLE_BUTTON, &CSmileyFaceDlg::OnBnClickedSampleButton)
	ON_BN_CLICKED(IDC_CLEAR_BUTTON, &CSmileyFaceDlg::OnBnClickedClearButton)
END_MESSAGE_MAP()


// CSmileyFaceDlg 消息处理程序

BOOL CSmileyFaceDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
//	pEdtConfidence = (CEdit*) GetDlgItem(IDC_EdtConfidence);
	pEdtConfidence.SetWindowText(L"70");
	pEdtConfidence.GetWindowText(strConfidence);
	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CSmileyFaceDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSmileyFaceDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSmileyFaceDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/*************打开图片函数**********************/


void CSmileyFaceDlg::OnBnClickedOpenPicButton()
{
	///////////////////dakaiwenjian
	CFileDialog dlg(TRUE,L"*.bmp", L"",OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_HIDEREADONLY,L"image files (*.bmp; *.jpg) |*.bmp;*.jpg|AVI files (*.avi) |*.avi|All Files (*.*)|*.*||",NULL);	
	dlg.m_ofn.lpstrTitle = L"请选择待检测照片";
	if (dlg.DoModal() == IDOK) 
	{
		path= dlg.GetPathName();
	    ShowPath(path);
		if (!image.IsNull()) {
		image.Destroy();
	};
	image.Load(path);
	int nWidth = image.GetWidth();
	int nHeight = image.GetHeight();
//	CRect rect;//定义矩形类
//	CRect rect1;
	m_Picture.GetClientRect(&rect); //获得pictrue控件所在的矩形区域
    pDc = m_Picture.GetDC();//获得pictrue控件的Dc
	SetStretchBltMode(pDc->m_hDC, STRETCH_HALFTONE);
	if (nWidth <= rect.Width() && nHeight <= rect.Width()) //小图片，不缩放
	{
		float xScale = (float)rect.Width() / (float)nWidth;
		float yScale = (float)rect.Height() / (float)nHeight;
		float ScaleIndex;
		if (xScale <= yScale)
			ScaleIndex = xScale;
		else
			ScaleIndex = yScale;
		int x = (int)((rect.Width()-nWidth*ScaleIndex)/2);
		int y = (int)((rect.Height()-nHeight*ScaleIndex)/2);
		CPoint rectCenter(x,y);
		rect_image = CRect(rectCenter, CSize((int)nWidth*ScaleIndex, (int)nHeight*ScaleIndex));
		//清除画布中原有图片
		CStatic* pStatic = (CStatic*)GetDlgItem(ID_IMAGEMAP);
        pStatic->SetBitmap(NULL);
	
		image.StretchBlt(pDc->m_hDC, rect_image, SRCCOPY); //将图片画到Picture控件表示的矩形区域

	}
	else
	{
		float xScale = (float)rect.Width() / (float)nWidth;
		float yScale = (float)rect.Height() / (float)nHeight;
		float ScaleIndex;
		if (xScale <= yScale)
			ScaleIndex = xScale;
		else
			ScaleIndex = yScale;
		int x = (int)((rect.Width()-nWidth*ScaleIndex)/2);
		int y = (int)((rect.Height()-nHeight*ScaleIndex)/2);
		CPoint rectCenter(x,y);
		rect_image = CRect(rectCenter, CSize((int)nWidth*ScaleIndex, (int)nHeight*ScaleIndex));
		//清除画布中原有图片
		pStatic = (CStatic*)GetDlgItem(ID_IMAGEMAP);
        pStatic->SetBitmap(NULL);
	
		image.StretchBlt(pDc->m_hDC, rect_image, SRCCOPY); //将图片画到Picture控件表示的矩形区域
	}
//	ReleaseDC(pDc);//释放picture控件的Dc
	}
}
/******************检测人脸函数**************************************/

void CSmileyFaceDlg::OnBnClickedStartButton()
{
	if(!path.IsEmpty()){
	pDc_face = m_Face_Image.GetDC();//获得face_pictrue控件的Dc           //此处将pdc修改为pDc_face，应判断是那个图片显示区域
	m_Face_Image.GetClientRect(&Facerect); //获得pictrue控件所在的矩形区域
	//Mat frame;
    Mat frame;  //设定要识别图片地址，此处前应该有获取图片的方法。
//	const std::string &fileName ="F:\\6.jpg" ;
	//Mat frame = imread(fileName);  //设定要识别图片地址，此处前应该有获取图片的方法。
	CImageToMat(image, frame);//通过Cimage拷贝到Mat
	//-- 1. Load the cascades
	bool isLoad1 = face_cascade.load( face_cascade_name );//加载脸部分类器
	bool isLoad2 =eyes_cascade.load( eyes_cascade_name );//加载眼睛分类器
	bool isLoad3= mouth_cascade.load( mouth_cascade_name );//加载嘴部分类器
//	if(isLoad1)
	if( !frame.empty() )
	{ detectAndDisplay(frame,pDc_face); }
	MatToCImage(frame, imageShow);
	imageShow.StretchBlt(pDc->m_hDC, rect_image, SRCCOPY); //将图片画到Picture控件表示的矩形区域 uuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuu
	ReleaseDC(pDc_face);//释放picture控件的Dc
	}else
	{
		AfxMessageBox(L"请先打开要检测的图片。");  //弹出对话框
	}
}

void detectAndDisplay( Mat& frame , CDC *pDc)
{
	int num_Mouths=0;
	int	num_Faces=0;
	int	num_Eyes=0;  //用于存放所检测到的数目
	std::vector<Rect> faces;
	std::vector<Rect> eyes;
	std::vector<Rect> mouth;
	Mat faceROI;
	Mat frame_gray;
	//设定8种颜色,zijia
	static CvScalar colors[8] = {
		{{0,0,255}},
		{{0,128,255}},
		{{0,255,255}},
		{{0,255,0}},
		{{255,128,0}},
		{{255,255,0}},
		{{255,0,0}},
		{{255,0,255}}
	};

	cvtColor( frame, frame_gray, CV_BGR2GRAY );//灰度处理
	imshow("原图", frame ); 
	imshow("灰度化处理", frame_gray ); 
	equalizeHist( frame_gray, frame_gray );//直方图均衡化
	imshow("直方图", frame_gray ); 
	//-- Detect faces
	face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );
//	cout<<faces.size()<<endl;
	//得到脸部图像，进行画圈
	for( size_t i = 0; i < (faces.size()?faces.size() : 0); i++ )
	{

		faceROI = frame_gray( faces[i] );//得到脸部图片
		//在脸部检测眼睛，如果没有眼睛则抛弃脸部
	//	cout<<faceROI.size()<<endl;
	
		eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30) ); //检测眼睛
		if(eyes.size()==0){                                                   //如果眼睛没检测到，则检测嘴
			mouth_cascade.detectMultiScale( faceROI, mouth, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30) ); //检测mouth
	//		cout<<"嘴个数"<<mouth.size()<<endl;
		for( int j = 0; j < (mouth.size()?mouth.size() : 0); j++ )
			{
				int m_int_x = int(faces[i].x + mouth[j].x + mouth[j].width*0.5);
				int m_int_y = int(faces[i].y + mouth[j].y + mouth[j].height*0.5);
				Point center = Point(m_int_x , m_int_y); 								
				int radius = cvRound( mouth[j].width*0.25 );		
	//			cout<<"mouth的纵坐标="<<mouth[j].y<<"  脸的高度"<<faces[i].height*0.5<<endl;
				if(mouth[j].y>faces[i].height*0.5){
				circle( frame, center, radius, colors[(i+1) % 8], 1, 3, 0 );  //画嘴
				num_Mouths++;//嘴数量+1
				}
				
		
		    }
		}else{

		try
		{
		//	cout<<"眼睛个数"<<eyes.size()<<endl;
			for( int j = 0; j < (eyes.size()?eyes.size() : 0); j++ )
			{
				int int_x = int(faces[i].x + eyes[j].x + eyes[j].width*0.5);
				int int_y = int(faces[i].y + eyes[j].y + eyes[j].height*0.5);
				Point center = Point(int_x , int_y); 
				int radius = cvRound( eyes[j].width*0.25 );
			//	cout<<"眼睛的y="<<eyes[j].y<<"眼睛的x="<<eyes[j].x<<"  脸的高度"<<faces[i].height*0.5<<endl;
				if(eyes[j].y<faces[i].height*0.5&&eyes[j].y>faces[i].height*0.2){
				circle( frame, center, radius, colors[(i+3) % 8], 3, 2, 0 );  //画眼睛
				num_Eyes++;//数量+1
				}
			//	Sleep(500);
			}
		}
		catch( cv::Exception& e )
		{
			const char* err_msg = e.what();
		//	printf ("%s\n", err_msg);
			
		}
		}
	        if(eyes.size()!=0||mouth.size()!=0||faces[i].height<80){
			Point center2( int(faces[i].x + faces[i].width*0.5), int(faces[i].y + faces[i].height*0.5) );  //找到脸部中点
			Point point1( int(faces[i].x), int(faces[i].y) );  //找到脸部中点
			Point point2( int(faces[i].x + faces[i].width), int(faces[i].y + faces[i].height) );  //找到脸部中点
		//	ellipse( frame, center2, Size( int(faces[i].width*0.5), int(faces[i].height*0.5)), 0, 0, 360,colors[i % 8], 2, 8, 0 );  //脸画圈
			rectangle(frame,point1,point2,colors[i % 8],3); //画脸
			    num_Faces++;//嘴数量+1
				CString str1,str2,str3,str4,str5;
                str1.Format(_T("%d"), num_Faces);
				str2.Format(_T("%d"), num_Eyes);
				str3.Format(_T("%d"), num_Mouths);
				str4="共检测到：\n";
				str4+=str1;str4+=" 张脸\n";
				str4+=str2;str4+=" 只眼\n";
				str4+=str3;str4+=" 张嘴";
				SetDlgItemText(AfxGetMainWnd()->m_hWnd,IDC_RESULT_TEXT,str4);	
				ShowFace(pDc,faceROI);  //显示脸aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
				if(doShowFace){
				str5="face";str5+=str1;
		        CStringW strw =str5;
                CStringA stra(strw.GetBuffer(0));
                strw.ReleaseBuffer();
                std::string facenum=stra.GetBuffer(0);
                stra.ReleaseBuffer();
		
			    imshow(facenum, faceROI );  
				/////////////////////////////////////////////////////////////////////测试
			//	std::string tmp = (LPSTR)(LPCTSTR)str5;
            //    const std::string &sFileName = tmp;
			//	imshow(sFileName, faceROI ); //测试
				/////////////////////////////////////////////////////////////////
				}
 	}
		///////////////////////////////////////////////////////////////////////////////////////
		//检测眼睛
		/*       std::vector<Rect> eyes;

		//-- In each face, detect eyes
		eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30) );

		for( size_t j = 0; j < eyes.size(); j++ )
		{
		Point center( int(faces[i].x + eyes[j].x + eyes[j].width*0.5), int(faces[i].y + eyes[j].y + eyes[j].height*0.5) ); 
		int radius = cvRound( (eyes[j].width + eyes[i].height)*0.25 );
		circle( frame, center, radius, Scalar( 255, 0, 0 ), 3, 8, 0 );
		}
		*/
	} 
/*	//-- Show what you got
	//imshow( "22222", frame );
	//显示在picture control中
	///////////////////dakaiwenjian
    MatToCImage(frame, imageShow);
	int nWidth = imageShow.GetWidth();
	int nHeight = imageShow.GetHeight();
//	CRect rect;//定义矩形类
//	CRect rect1;
//	m_Picture.GetClientRect(&rect); //获得pictrue控件所在的矩形区域
//	CDC *pDc = m_Picture.GetDC();//获得pictrue控件的Dc
	SetStretchBltMode(pDc->m_hDC, STRETCH_HALFTONE);
	if (nWidth <= rect.Width() && nHeight <= rect.Width()) //小图片，不缩放
	{
		rect1 = CRect(rect.TopLeft(), CSize(nWidth, nHeight));
		imageShow.StretchBlt(pDc->m_hDC, rect1, SRCCOPY); //将图片画到Picture控件表示的矩形区域

	}
	else
	{
		float xScale = (float)rect.Width() / (float)nWidth;
		float yScale = (float)rect.Height() / (float)nHeight;
		float ScaleIndex;
		if (xScale <= yScale)
			ScaleIndex = xScale;
		else
			ScaleIndex = yScale;
		int x = (int)((rect.Width()-nWidth*ScaleIndex)/2);
		int y = (int)((rect.Height()-nHeight*ScaleIndex)/2);
		CPoint rectCenter(x,y);
		rect1 = CRect(rectCenter, CSize((int)nWidth*ScaleIndex, (int)nHeight*ScaleIndex));
		//清除画布中原有图片
	//	CStatic* pStatic = (CStatic*)GetDlgItem(ID_IMAGEMAP);
    //    pStatic->SetBitmap(NULL);
	
		imageShow.StretchBlt(pDc->m_hDC, rect1, SRCCOPY); //将图片画到Picture控件表示的矩形区域
	}
*/
	
	CString str1,str2,str3,str4,str5;
                str1.Format(_T("%d"), num_Faces);
				str2.Format(_T("%d"), num_Eyes);
				str3.Format(_T("%d"), num_Mouths);
				str4="共检测到：\n";
				str4+=str1;str4+=" 张脸\n";
				str4+=str2;str4+=" 只眼\n";
				str4+=str3;str4+=" 张嘴";
				SetDlgItemText(AfxGetMainWnd()->m_hWnd,IDC_RESULT_TEXT,str4);	
//	imshow( "脸部检测结果", frame );                //      hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh
	
	}




void ShowFace(CDC *pDc,Mat faceROI)
{   CImage faceImage;
	MatToCImage(faceROI, faceImage);
	int nWidth = faceImage.GetWidth();
	int nHeight = faceImage.GetHeight();
	SetStretchBltMode(pDc->m_hDC, STRETCH_HALFTONE);
	if (nWidth <= Facerect.Width() && nHeight <= Facerect.Width()) //小图片，不缩放
	{
		float xScale = (float)Facerect.Width() / (float)nWidth;
		float yScale = (float)Facerect.Height() / (float)nHeight;
		float ScaleIndex;
		if (xScale <= yScale)
			ScaleIndex = xScale;
		else
			ScaleIndex = yScale;
		int x = (int)((Facerect.Width()-nWidth*ScaleIndex)/2);
		int y = (int)((Facerect.Height()-nHeight*ScaleIndex)/2);
		CPoint rectCenter(x,y);
		rect_face = CRect(rectCenter, CSize((int)nWidth*ScaleIndex, (int)nHeight*ScaleIndex));
		//清除画布中原有图片
	//	CStatic* pStatic = (CStatic*)GetDlgItem(AfxGetMainWnd()->m_hWnd,IDC_FACE_IMAGE);
    //    pStatic->SetBitmap(NULL);
	
		faceImage.StretchBlt(pDc->m_hDC, Facerect, SRCCOPY); //将图片画到Picture控件表示的矩形区域

	}
	else
	{
		float xScale = (float)Facerect.Width() / (float)nWidth;
		float yScale = (float)Facerect.Height() / (float)nHeight;
		float ScaleIndex;
		if (xScale <= yScale)
			ScaleIndex = xScale;
		else
			ScaleIndex = yScale;
		int x = (int)((Facerect.Width()-nWidth*ScaleIndex)/2);
		int y = (int)((Facerect.Height()-nHeight*ScaleIndex)/2);
		CPoint rectCenter(x,y);
		rect_face = CRect(rectCenter, CSize((int)nWidth*ScaleIndex, (int)nHeight*ScaleIndex));
		//清除画布中原有图片
	//	CStatic* pStatic = (CStatic*)GetDlgItem(AfxGetMainWnd()->m_hWnd,IDC_FACE_IMAGE);
  //      pStatic->SetBitmap(NULL);
	
//		faceImage.StretchBlt(pDc->m_hDC, Facerect, SRCCOPY); //将图片画到Picture控件表示的矩形区域
		faceImage.StretchBlt(pDc->m_hDC, rect_face, SRCCOPY); //将图片画到Picture控件表示的矩形区域
	}
//	ReleaseDC(pDc);//释放picture控件的Dc
}








void MatToCImage(Mat& mat, CImage& cimage)
{
	if (0 == mat.total())
	{
		return;
	}


	int nChannels = mat.channels();
	if ((1 != nChannels) && (3 != nChannels))
	{
		return;
	}
	int nWidth    = mat.cols;
	int nHeight   = mat.rows;


	//重建cimage
	cimage.Destroy();
	cimage.Create(nWidth, nHeight, 8 * nChannels);


	//拷贝数据


	uchar* pucRow;									//指向数据区的行指针
	uchar* pucImage = (uchar*)cimage.GetBits();		//指向数据区的指针
	int nStep = cimage.GetPitch();					//每行的字节数,注意这个返回值有正有负


	if (1 == nChannels)								//对于单通道的图像需要初始化调色板
	{
		RGBQUAD* rgbquadColorTable;
		int nMaxColors = 256;
		rgbquadColorTable = new RGBQUAD[nMaxColors];
		cimage.GetColorTable(0, nMaxColors, rgbquadColorTable);
		for (int nColor = 0; nColor < nMaxColors; nColor++)
		{
			rgbquadColorTable[nColor].rgbBlue = (uchar)nColor;
			rgbquadColorTable[nColor].rgbGreen = (uchar)nColor;
			rgbquadColorTable[nColor].rgbRed = (uchar)nColor;
		}
		cimage.SetColorTable(0, nMaxColors, rgbquadColorTable);
		delete []rgbquadColorTable;
	}


	for (int nRow = 0; nRow < nHeight; nRow++)
	{
		pucRow = (mat.ptr<uchar>(nRow));
		for (int nCol = 0; nCol < nWidth; nCol++)
		{
			if (1 == nChannels)
			{
				*(pucImage + nRow * nStep + nCol) = pucRow[nCol];
			}
			else if (3 == nChannels)
			{
				for (int nCha = 0 ; nCha < 3; nCha++)
				{
					*(pucImage + nRow * nStep + nCol * 3 + nCha) = pucRow[nCol * 3 + nCha];
				}			
			}
		}	
	}
}

void CImageToMat(CImage& cimage, Mat& mat)
{
	if (true == cimage.IsNull())
	{
		return;
	}


	int nChannels = cimage.GetBPP() / 8;
	if ((1 != nChannels) && (3 != nChannels))
	{
		return;
	}
	int nWidth    = cimage.GetWidth();
	int nHeight   = cimage.GetHeight();


	//重建mat
	if (1 == nChannels)
	{
		mat.create(nHeight, nWidth, CV_8UC1);
	}
	else if(3 == nChannels)
	{
		mat.create(nHeight, nWidth, CV_8UC3);
	}


	//拷贝数据


	uchar* pucRow;									//指向数据区的行指针
	uchar* pucImage = (uchar*)cimage.GetBits();		//指向数据区的指针
	int nStep = cimage.GetPitch();					//每行的字节数,注意这个返回值有正有负


	for (int nRow = 0; nRow < nHeight; nRow++)
	{
		pucRow = (mat.ptr<uchar>(nRow));
		for (int nCol = 0; nCol < nWidth; nCol++)
		{
			if (1 == nChannels)
			{
				pucRow[nCol] = *(pucImage + nRow * nStep + nCol);
			}
			else if (3 == nChannels)
			{
				for (int nCha = 0 ; nCha < 3; nCha++)
				{
					pucRow[nCol * 3 + nCha] = *(pucImage + nRow * nStep + nCol * 3 + nCha);
				}			
			}
		}	
	}
}

//测试


void CSmileyFaceDlg::OnBnClickedExitButton()
{
	
}


void CSmileyFaceDlg::OnBnClickedButton1()
{ 
	Mat frame;

	//-- 1. Load the cascades
	if(face_cascade.empty())
	face_cascade.load( face_cascade_name );

	///////////////////dakaiwenjian
	CFileDialog dlg(TRUE,L"*.avi", L"",OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_HIDEREADONLY,L"AVI files (*.avi) |*.avi;*.mp4;*.3gp;*.rmvb|All Files (*.*)|*.*||",NULL);	
	dlg.m_ofn.lpstrTitle = L"请选择待检测视频文件";
	if (dlg.DoModal() == IDOK) 
	{
		path= dlg.GetPathName();
		ShowPath(path);
	/*	  CStringW strw =path;
    CStringA stra(strw.GetBuffer(0));
    strw.ReleaseBuffer();
    std::string imgpath=stra.GetBuffer(0);
    stra.ReleaseBuffer();

	*/
		////////////////////////////////////////////////////////////////
		//测试
		std::string tmp = (LPSTR)(LPCTSTR)path;
        const std::string &sFileName = tmp;




		///////////////////////////////////////////////////
//	VideoCapture capture("Simple.avi");
    VideoCapture capture(sFileName);///////////////////////////ceshi
//	VideoCapture capture(path);
	if( capture.isOpened() )	// 摄像头读取文件开关
	{
		while( true )
		{
 		//	frame = cvQueryFrame( capture );	// 摄像头读取文件开关
			capture >> frame;

			//-- 3. Apply the classifier to the frame
			if( !frame.empty() )
			{ detectAndDisplay2( frame,1 ); }
			else
			{ printf(" --(!) No captured frame -- Break!"); break; }

			int c = waitKey(10);
			if( (char)c == 'c' ) {  break; } 

		}
	}
	}
	cvDestroyWindow("视频文件人脸检测");
}

/***********开启摄像头******************/

//此处做大量修改
void CSmileyFaceDlg::OnBnClickedCamButton()
{
///**********************************************************************************************8

	takeSample = false;
	if(CloseThread){
	m_Picture.GetClientRect(&rect); //获得pictrue控件所在的矩形区域
    pDc = m_Picture.GetDC();//获得pictrue控件的Dc
	pDc_face = m_Face_Image.GetDC();//获得pictrue控件的Dc
	m_Face_Image.GetClientRect(&Facerect); //获得pictrue控件所在的矩形区域
	SetStretchBltMode(pDc->m_hDC, STRETCH_HALFTONE);
	SetStretchBltMode(pDc_face->m_hDC, STRETCH_HALFTONE);
	//此处应该判断pdc是否为空，否则会出错，还应加入线程控制机制，boole 来控制线程结束，改变按钮文字。
	CStatic* pStatic = (CStatic*)GetDlgItem(ID_IMAGEMAP);	
    pStatic->SetBitmap(NULL);
	pStatic = (CStatic*)GetDlgItem(IDC_FACE_IMAGE);	
    pStatic->SetBitmap(NULL);
	CloseThread = false; //用于改变线程状态；true时线程关闭。
    CString str1;
    str1 = "关闭摄像头";
	SetDlgItemText(IDC_CAM_BUTTON,str1);
	//此处应该将识别、采集素材。两个按钮变为可控
    ::AfxBeginThread(ThreadFun,NULL);  //开启线程
	GetDlgItem(IDC_RECOGNIZE_BUTTON)->EnableWindow(true);
	GetDlgItem(IDC_SAMPLE_BUTTON)->EnableWindow(true);
    str1="摄像头已开启";
	SetDlgItemText(IDC_STATE_TEXT,str1);
	}else
	{
	GetDlgItem(IDC_SAMPLE_BUTTON)->EnableWindow(false);
	GetDlgItem(IDC_RECOGNIZE_BUTTON)->EnableWindow(false);
	CString str1;
    str1 = "开启摄像头";
	SetDlgItemText(IDC_CAM_BUTTON,str1);
	CloseThread = true; //用于改变线程状态；true时线程关闭。
	//此处应该将识别、采集素材。两个按钮变为不可控
	cvReleaseCapture(&pCapture);
	str1="摄像头已关闭";
	SetDlgItemText(IDC_STATE_TEXT,str1);
//	isFirst= true;
	cam_frame.release();
	}





/////////////////////////////////////////****************************************************************
	/*
	if(face_cascade.empty())
	face_cascade.load( face_cascade_name );

		//声明IplImage指针  
 // IplImage* pFrame = NULL;  
  Mat cam_frame;
 //获取摄像头  
  CvCapture* pCapture = cvCreateCameraCapture(0);  
   
  //创建窗口  
 // cvNamedWindow("video", 1);  
   
  //显示视屏  
  while(1)  
  {  cam_frame=cvQueryFrame( pCapture );  
    //  pFrame=cvQueryFrame( pCapture );
  Sleep(3000);
      if(cam_frame.empty())break;  
     // cvShowImage("video",pFrame);
	  detectAndDisplay2( cam_frame,2 );
      
	  char c=cvWaitKey(33);  
      if(c==27)break;  
  }  
  cvReleaseCapture(&pCapture);  
  cvDestroyWindow("摄像头实时监测");
  */
	/*****改进算法*******/
///////////////////////////////////////////////////////////////////
	//测试，以下代码先注释掉
	////////////////////////////////////////////////////////
	/*
if(face_cascade.empty())
	face_cascade.load( face_cascade_name );

		//声明IplImage指针  
 // IplImage* pFrame = NULL;  
  Mat cam_frame;
   CvCapture* pCapture;
   bool isFirst = true;
  while(1)  
  { 
	
while(cam_frame.empty()&&isFirst){
 //获取摄像头  
 pCapture = cvCreateCameraCapture(0);  
  //显示视屏  
   cam_frame=cvQueryFrame( pCapture );
   isFirst= false;
}
         cam_frame=cvQueryFrame( pCapture );
    //  pFrame=cvQueryFrame( pCapture );
    //  Sleep(3000);
  //    if(cam_frame.empty()) 
     // cvShowImage("video",pFrame);
	  detectAndDisplay2( cam_frame,2 );  //cam_frame 为传引用。
      
	  char c=cvWaitKey(33);  
      if(c==27)break;  
  }  
  cvReleaseCapture(&pCapture);
  */
  ///////////////////////////////////////////////////////////////////
	//测试，以下代码先注释掉
	////////////////////////////////////////////////////////

//  cvDestroyWindow("摄像头实时监测");

/*****改进算法*******/
	/*	Mat cam_frame;
	//-- 1. Load the cascades
//	face_cascade.load( face_cascade_name );
	//eyes_cascade.load( eyes_cascade_name ); 
	//-- 2. Read the video stream
 	CvCapture* capture = cvCaptureFromCAM( 0 );	// 摄像头读取文件开关
//	VideoCapture capture("Sample.avi");
//	if( capture.isOpened() )	// 摄像头读取文件开关
//	{
		while( true )
		{
 			cam_frame = cvQueryFrame( capture );	// 摄像头读取文件开关
		//	capture >> frame;

			//-- 3. Apply the classifier to the frame
			if( !cam_frame.empty() )
			{// detectAndDisplay2( cam_frame ); 
	Sleep(2000);
				imshow( "1111", cam_frame );
			}

		}
		8*/
}


/********************实时人脸检测函数********************************/
//画脸后返回Mat引用
void detectAndDisplay2( Mat &cam_frame ,int type )
{
	std::vector<Rect> faces;
	Mat frame_gray;
	Mat faceROI;//人脸截图
	cvtColor( cam_frame, frame_gray, CV_BGR2GRAY );
	equalizeHist( frame_gray, frame_gray );
	//-- Detect faces
	face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );

	for( size_t i = 0; i < faces.size(); i++ )
	{
		Point center( int(faces[i].x + faces[i].width*0.5), int(faces[i].y + faces[i].height*0.5) );
	//ellipse( cam_frame, center, Size( int(faces[i].width*0.5), int(faces[i].height*0.5)), 0, 0, 360, Scalar( 255, 0, 255 ), 2, 8, 0 );
     
			Point point1( int(faces[i].x), int(faces[i].y) );  //找到脸部中点
			Point point2( int(faces[i].x + faces[i].width), int(faces[i].y + faces[i].height) );  //找到脸部中点
		
			rectangle(cam_frame,point1,point2,Scalar( 255, 0, 255 ),3); //画脸
	        faceROI = frame_gray( faces[i] );

			ShowFace(pDc_face,faceROI);
			if(takeSample==2){  //如果取样，则保存图片
	
	//Mat img(frame_face_gray,0);
	stringstream ss;
	ss << (read_img_number()+1);
	sampleNum = read_img_number()+1;
	string faceImgName = "..//einfacedata//trainingdata//"+ss.str()+".jpg";
	imwrite(faceImgName,faceROI);
	takeSample = 0;
	//pTip->GetWindowText(strTip);
// 	CString tipPhoto = strTip + "\r\n拍照成功！已存为" + ("/einfacedata/trainingdata/"+ss.str()+".jpg").c_str();
	//pTip->SetWindowText( tipPhoto );
	//MessageBox("OK");
			}

			
			/*	Mat faceROI = frame_gray( faces[i] );
		std::vector<Rect> eyes;

		//-- In each face, detect eyes
		eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30) );

		for( size_t j = 0; j < eyes.size(); j++ )
		{
			Point center( int(faces[i].x + eyes[j].x + eyes[j].width*0.5), int(faces[i].y + eyes[j].y + eyes[j].height*0.5) ); 
			int radius = cvRound( (eyes[j].width + eyes[i].height)*0.25 );
			circle( frame, center, radius, Scalar( 255, 0, 0 ), 3, 8, 0 );
		}
		*/
	} 


	//-- Show what you got
//	if(2==type)
//	imshow( "摄像头实时监测", cam_frame );
//	else
//	imshow( "视频文件人脸检测", cam_frame );

}

void CSmileyFaceDlg::OnBnClickedCheck1()
{
	if(BST_UNCHECKED==m_CheckBox.GetCheck())
		doShowFace = false;
	else
		doShowFace = true;
}

void ShowPath(CString path){
//CString str1,str2,str3,str4,str5;
 //               str1.Format(_T("%d"), num_Faces);
	//			str2.Format(_T("%d"), num_Eyes);
	//			str3.Format(_T("%d"), num_Mouths);
	//			str4="共检测到：\n\n";
	//			str4+=str1;str4+=" 张脸\n\n";
		//		str4+=str2;str4+=" 只眼\n\n";
		//		str4+=str3;str4+=" 张嘴";
	CString str1;
str1 = "当前文件路径：";
	str1+=path;
				SetDlgItemText(AfxGetMainWnd()->m_hWnd,IDC_STATE_TEXT,str1);	

}


///////////////////////////////////////////////////////////////////////////











//应该采用多线程。

//人脸识别函数!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void CSmileyFaceDlg::OnBnClickedRecognizeButton()
{                                            //do false   train false    正常不识别状态
	                                         //do true    train false    正常识别状态
	                                         //do false   train ture     训练失败状态
	if(trainErro){
    trainErro=false;
	doRecognize = false;
    CString str1;
    str1 = "开始识别";
	SetDlgItemText(IDC_RECOGNIZE_BUTTON,str1);
	GetDlgItem(IDC_SAMPLE_BUTTON)->EnableWindow(true);
	}else
	if(!doRecognize){
		GetDlgItem(IDC_SAMPLE_BUTTON)->EnableWindow(false);
	pEdtConfidence.GetWindowText(strConfidence);
	try
	{
//		CString str=_T("这是我的测试程序。");
// 先得到要转换为字符的长度
    const size_t strsize=(strConfidence.GetLength()+1)*2; // 宽字符的长度;
    char * pstr= new char[strsize]; //分配空间;
    size_t sz=0;
    wcstombs_s(&sz,pstr,strsize,strConfidence,_TRUNCATE);

	dConfidence = atoi((const char*)pstr);	
		
	}
	catch(cv::Exception &e)
	{
		MessageBox(L"置信度请输入整数！");
		return;
	}
	
	model->set("threshold", dConfidence);
	doRecognize = true;
	CString str1;
    str1 = "停止识别";
	SetDlgItemText(IDC_RECOGNIZE_BUTTON,str1);
	GetDlgItem(IDC_SAMPLE_BUTTON)->EnableWindow(false);
	}else{
		GetDlgItem(IDC_SAMPLE_BUTTON)->EnableWindow(true);
	doRecognize = false;
    CString str1;
    str1 = "开始识别";
	SetDlgItemText(IDC_RECOGNIZE_BUTTON,str1);
	}
	/*
	if(CloseThread){
	m_Picture.GetClientRect(&rect); //获得pictrue控件所在的矩形区域
    pDc = m_Picture.GetDC();//获得pictrue控件的Dc
	SetStretchBltMode(pDc->m_hDC, STRETCH_HALFTONE);
	//此处应该判断pdc是否为空，否则会出错，还应加入线程控制机制，boole 来控制线程结束，改变按钮文字。
	CStatic* pStatic = (CStatic*)GetDlgItem(ID_IMAGEMAP);	
    pStatic->SetBitmap(NULL);
	CloseThread = false; //用于改变线程状态；true时线程关闭。
    CString str1;
    str1 = "停止检测";
	SetDlgItemText(IDC_RECOGNIZE_BUTTON,str1);
    ::AfxBeginThread(ThreadFun,NULL);  
	}else
	{
	CString str1;
    str1 = "面部识别";
	SetDlgItemText(IDC_RECOGNIZE_BUTTON,str1);
	CloseThread = true; //用于改变线程状态；true时线程关闭。
	}
	*/


}


UINT WINAPI CSmileyFaceDlg::ThreadFun(LPVOID lpParam){


	if(face_cascade.empty())
	face_cascade.load( face_cascade_name );
    //声明IplImage指针  
   pFrame = NULL;  
 //  Mat cam_frame;
 //  CvCapture* pCapture;
   isFirst = true;
   while(1)  
 { 	
  while(cam_frame.empty()&&isFirst){
 //获取摄像头  
   pCapture = cvCreateCameraCapture(0);  
  //显示视屏  
   cam_frame=cvQueryFrame( pCapture );
   isFirst= false;
  }
  if(doRecognize){   //判断是否识别人脸
      pFrame=cvQueryFrame( pCapture );
      
      if(!recog_and_draw_new(pFrame,face_cascade,takeSample,sampleNum)){//检测和识别人脸	
	  //如果识别出错，文件读取出错，则将doRecognize设置为false
	  doRecognize = false;
	  trainErro = true;
    //  CString str1;
   //   str1 = "开始识别";
	//  SetDlgItemText(AfxGetMainWnd()->m_hWnd,IDC_RECOGNIZE_BUTTON,str1);
	//  SetDlgItemText(AfxGetMainWnd()->m_hWnd,IDC_RECOGNIZE_BUTTON,str1);
	  }  
	  
	  cam_frame = pFrame;
	  //改进方法，将摄像头捕获图像显示在picture控件上	
	  MatToCImage(cam_frame, imageShow);//将识别好的mat转换为CImage
	  int nWidth = imageShow.GetWidth();
	  int nHeight = imageShow.GetHeight();
   	CRect rect_CamRecognize;//定义矩形类 用于保存摄像头图片大小

	if (nWidth <= rect.Width() && nHeight <= rect.Width()) //小图片，不缩放
	{
		float xScale = (float)rect.Width() / (float)nWidth;
		float yScale = (float)rect.Height() / (float)nHeight;
		float ScaleIndex;
		if (xScale <= yScale)
			ScaleIndex = xScale;
		else
			ScaleIndex = yScale;
		int x = (int)((rect.Width()-nWidth*ScaleIndex)/2);
		int y = (int)((rect.Height()-nHeight*ScaleIndex)/2);
		CPoint rectCenter(x,y);
		rect_CamRecognize = CRect(rectCenter, CSize((int)nWidth*ScaleIndex, (int)nHeight*ScaleIndex));
		//清除画布中原有图片
	//	CStatic* pStatic = (CStatic*)GetDlgItem(AfxGetMainWnd()->m_hWnd,ID_IMAGEMAP);
	//	CStatic* pStatic = (CStatic*)GetDlgItem(ID_IMAGEMAP,lpParam);
   //     pStatic->SetBitmap(NULL);
	
		imageShow.StretchBlt(pDc->m_hDC, rect_CamRecognize, SRCCOPY); //将图片画到Picture控件表示的矩形区域

	}
	else
	{
		float xScale = (float)rect.Width() / (float)nWidth;
		float yScale = (float)rect.Height() / (float)nHeight;
		float ScaleIndex;
		if (xScale <= yScale)
			ScaleIndex = xScale;
		else
			ScaleIndex = yScale;
		int x = (int)((rect.Width()-nWidth*ScaleIndex)/2);
		int y = (int)((rect.Height()-nHeight*ScaleIndex)/2);
		CPoint rectCenter(x,y);
		rect_CamRecognize = CRect(rectCenter, CSize((int)nWidth*ScaleIndex, (int)nHeight*ScaleIndex));
		//清除画布中原有图片
	//	pStatic = (CStatic*)GetDlgItem(AfxGetMainWnd()->m_hWnd,ID_IMAGEMAP);
     //   pStatic->SetBitmap(NULL);
	
		imageShow.StretchBlt(pDc->m_hDC, rect_CamRecognize, SRCCOPY); //将图片画到Picture控件表示的矩形区域
	}
	  }
	  else{    //仅仅检测人脸
	 // Mat findFaceResult = pFrame;
		  cam_frame=cvQueryFrame( pCapture );
      detectAndDisplay2( cam_frame ,1);//第二个参数没用  
	   //改进方法，将摄像头捕获图像显示在picture控件上	
	  MatToCImage(cam_frame, imageShow);//将识别好的mat转换为CImage
	  int nWidth = imageShow.GetWidth();
	  int nHeight = imageShow.GetHeight();
   	CRect rect_CamRecognize;//定义矩形类 用于保存摄像头图片大小

	if (nWidth <= rect.Width() && nHeight <= rect.Width()) //小图片，不缩放
	{
		float xScale = (float)rect.Width() / (float)nWidth;
		float yScale = (float)rect.Height() / (float)nHeight;
		float ScaleIndex;
		if (xScale <= yScale)
			ScaleIndex = xScale;
		else
			ScaleIndex = yScale;
		int x = (int)((rect.Width()-nWidth*ScaleIndex)/2);
		int y = (int)((rect.Height()-nHeight*ScaleIndex)/2);
		CPoint rectCenter(x,y);
		rect_CamRecognize = CRect(rectCenter, CSize((int)nWidth*ScaleIndex, (int)nHeight*ScaleIndex));
		//清除画布中原有图片
	//	CStatic* pStatic = (CStatic*)GetDlgItem(AfxGetMainWnd()->m_hWnd,ID_IMAGEMAP);
	//	CStatic* pStatic = (CStatic*)GetDlgItem(ID_IMAGEMAP,lpParam);
   //     pStatic->SetBitmap(NULL);
	
		imageShow.StretchBlt(pDc->m_hDC, rect_CamRecognize, SRCCOPY); //将图片画到Picture控件表示的矩形区域

	}
	else
	{
		float xScale = (float)rect.Width() / (float)nWidth;
		float yScale = (float)rect.Height() / (float)nHeight;
		float ScaleIndex;
		if (xScale <= yScale)
			ScaleIndex = xScale;
		else
			ScaleIndex = yScale;
		int x = (int)((rect.Width()-nWidth*ScaleIndex)/2);
		int y = (int)((rect.Height()-nHeight*ScaleIndex)/2);
		CPoint rectCenter(x,y);
		rect_CamRecognize = CRect(rectCenter, CSize((int)nWidth*ScaleIndex, (int)nHeight*ScaleIndex));
		//清除画布中原有图片
	//	pStatic = (CStatic*)GetDlgItem(AfxGetMainWnd()->m_hWnd,ID_IMAGEMAP);
     //   pStatic->SetBitmap(NULL);
	
		imageShow.StretchBlt(pDc->m_hDC, rect_CamRecognize, SRCCOPY); //将图片画到Picture控件表示的矩形区域



	  }
}
	if(CloseThread)
		break;
  

  

}

 return 0;

}
/**
*采样函数
*/
void CSmileyFaceDlg::OnBnClickedSampleButton()
{	/*
	if(takeSample==1||takeSample==0){
	takeSample = 2;
	CString str1;
    str1 = "停止采样";
	SetDlgItemText(IDC_SAMPLE_BUTTON,str1);
	}else
	{
	takeSample = 0;
	CString str1;
    str1 = "采集样本";
	SetDlgItemText(IDC_SAMPLE_BUTTON,str1);	
	if(sampleNum!=-1){
	    CString str1,str2;
	    str1="训练库中样本数量：";
		str2.Format(_T("%d"), sampleNum);
	    str1 += str2;
	    SetDlgItemText(IDC_STATE_TEXT,str1);
	  }
	}
	*/
	takeSample = 2;
	if(sampleNum!=-1){
	    CString str1,str2;
	    str1="训练库中样本数量：";
		str2.Format(_T("%d"), sampleNum);
	    str1 += str2;
	    SetDlgItemText(IDC_STATE_TEXT,str1);
	  }
}


void CSmileyFaceDlg::OnBnClickedClearButton()
{
	// TODO: 在此添加控件通知处理程序代码
	if(delete_img())
	{
		AfxMessageBox(L"删除成功！");
		sampleNum = 0;
		 CString str1,str2;
	    str1="训练库中样本数量：";
		str2.Format(_T("%d"), sampleNum);
	    str1 += str2;
	    SetDlgItemText(IDC_STATE_TEXT,str1);
	}
	else
	{
		AfxMessageBox(L"删除失败！");
	}
}



