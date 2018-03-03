
// SmileyFaceDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
//////////////////////////////////////////、、、、、面部识别部分
//#include <opencv2/highgui/highgui_c.h>
//#include "cv.h"
//#include "highgui.h"
#include "CvvImage.h"
#include "detect_recog.h"
//#include <conio.h>

////////////////////////////////////////////////////////////////////
// CSmileyFaceDlg 对话框
class CSmileyFaceDlg : public CDialogEx
{
// 构造
public:
	CSmileyFaceDlg(CWnd* pParent = NULL);	// 标准构造函数
//	static bool takeSample;
// 对话框数据
	enum { IDD = IDD_SMILEYFACE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
//	virtual ~CSmileyFaceDlg();

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
//	static CvCapture * pCapture;
	afx_msg void OnBnClickedOpenPicButton();
	CStatic m_Picture;
	afx_msg void OnBnClickedStartButton();
	CStatic m_Face_Image;
	CString m_Result_Text;
	afx_msg void OnBnClickedExitButton();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedCamButton();
	CButton m_CheckBox;
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedRecognizeButton();
	//afx_msg void OnTimer(UINT_PTR nIDEvent);
	//UINT ThreadFun(); //多线程
	static UINT WINAPI ThreadFun(LPVOID lpParam);
	afx_msg void OnBnClickedSampleButton();
	afx_msg void OnBnClickedClearButton();
	CEdit pEdtConfidence;
};

