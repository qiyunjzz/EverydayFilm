#include "StdAfx.h"
#include "SetDialog.h"

void CSetDialog::OnFinalMessage(HWND /*hWnd*/)
{
	delete this;
}

void CSetDialog::Init()
{
	TCHAR path[MAX_PATH] = {0};
	::GetModuleFileName(NULL,path,MAX_PATH);
	m_cfgPath = path;            //获得程序路径

	m_strIcoPath=m_strExePath=path;
	m_cfgPath.Replace(_T("FilmEveryday.exe"),_T("ttkvodConfig.db"));
	m_strIcoPath.Replace(_T("FilmEveryday.exe"),_T("icons\\"));
	m_strIcoPath.MakeLower();
	//m_strExePath.MakeLower();

	OSVERSIONINFO OsVersion;
	ZeroMemory(&OsVersion, sizeof(OSVERSIONINFO));
	OsVersion.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);   
	::GetVersionEx(&OsVersion);
	if(OsVersion.dwMajorVersion==6 || OsVersion.dwMajorVersion==7)//vista或win7
	{
		LPITEMIDLIST pidl;
		SHGetSpecialFolderLocation(NULL,CSIDL_COMMON_APPDATA,&pidl);
		if(pidl)
		{
			TCHAR szPathName[MAX_PATH];
			SHGetPathFromIDList(pidl,szPathName); 
			m_qvodiniPath = szPathName;
			m_qvodiniPath += _T("\\QvodPlayer\\QvodCfg.ini");
			LPMALLOC pMalloc;
			SHGetMalloc(&pMalloc); 
			pMalloc->Free(pidl); 
		}
		else
		{
			m_qvodiniPath = path;
			m_qvodiniPath.Replace(_T("FilmEveryday.exe"),_T("QvodCfg.ini"));
		}
	}
	else
	{
		m_qvodiniPath = path;
		m_qvodiniPath.Replace(_T("FilmEveryday.exe"),_T("QvodCfg.ini"));
	}


	SHGetSpecialFolderPath(0,path,CSIDL_DESKTOPDIRECTORY,0);
	m_strDesttop = path;
	m_strDesttop += _T("\\ttk_image\\");//获得默认文件夹

	::GetPrivateProfileString(_T("memory"),_T("picpath"),_T("nopath"),path,MAX_PATH,m_cfgPath);//从文件查找

	if( _tcscmp(path, _T("nopath")) == 0 )//文件中没用，使用默认的
	{
		::WritePrivateProfileString(_T("memory"),_T("picpath"),m_strDesttop,m_cfgPath);
	}
	////////////////获得系统版本//////////////////////////////////

	m_btnClose = static_cast<CButtonUI*>(m_pm->FindControl(_T("btnClose")));
	m_btnChangePath = static_cast<CButtonUI*>(m_pm->FindControl(_T("change")));
	m_btnChangePicpath = static_cast<CButtonUI*>(m_pm->FindControl(_T("change2")));

	m_btnSure = static_cast<CButtonUI*>(m_pm->FindControl(_T("sure")));
	m_btnGiveup = static_cast<CButtonUI*>(m_pm->FindControl(_T("giveup")));

	m_btnAbout = static_cast<COptionUI*>(m_pm->FindControl(_T("aboutus")));
	m_btnPower = static_cast<COptionUI*>(m_pm->FindControl(_T("timeauto")));
	m_btnPicSet = static_cast<COptionUI*>(m_pm->FindControl(_T("imageset")));
	m_btnPathSet = static_cast<COptionUI*>(m_pm->FindControl(_T("pathset")));

	int alws = ::GetPrivateProfileInt(_T("memory"),_T("always"),0,m_cfgPath);

	pbrightness = static_cast<CSliderUI*>(m_pm->FindControl(_T("sliderbrightness")));
	pbrightness->SetMaxValue(512);
	pbrightness->SetValue(alws?::GetPrivateProfileInt(_T("cfg"),_T("brightness"),256,m_cfgPath):256);

	saturation = static_cast<CSliderUI*>(m_pm->FindControl(_T("slidersaturation")));
	saturation->SetMaxValue(128);
	saturation->SetValue(alws?::GetPrivateProfileInt(_T("cfg"),_T("saturation"),64,m_cfgPath):64);

	contrast = static_cast<CSliderUI*>(m_pm->FindControl(_T("slidercontrast")));
	contrast->SetMaxValue(256);
	contrast->SetValue(alws?::GetPrivateProfileInt(_T("cfg"),_T("contrast"),128,m_cfgPath):128);

	hue = static_cast<CSliderUI*>(m_pm->FindControl(_T("sliderhue")));
	hue->SetMaxValue(360);
	hue->SetValue(alws?::GetPrivateProfileInt(_T("cfg"),_T("hue"),180,m_cfgPath):180);

	m_pm->SendNotify(NULL,MST_VALUECHANGED);

	suavity = static_cast<CButtonUI*>(m_pm->FindControl(_T("suavity")));
	bright = static_cast<CButtonUI*>(m_pm->FindControl(_T("brightness")));
	restore = static_cast<CButtonUI*>(m_pm->FindControl(_T("restore")));

	//加入时间
	CComboUI *pHour = static_cast<CComboUI*>(m_pm->FindControl(_T("hour")));
	for(int i=0;i<24;i++)
	{
		CListLabelElementUI *pElement = new CListLabelElementUI;
		CStdString a;
		a.Format(_T("%d"),i);
		pElement->SetText(a);
		pHour->Add(pElement);
	}
	CComboUI *pMinute = static_cast<CComboUI*>(m_pm->FindControl(_T("minute")));
	for(int i=0;i<60;i++)
	{
		CListLabelElementUI *pElement = new CListLabelElementUI;
		CStdString a;
		a.Format(_T("%d"),i);
		pElement->SetText(a);
		pMinute->Add(pElement);
	}

	////////////文件关联
	m_btnallFileAssociations = static_cast<CButtonUI*>(m_pm->FindControl(_T("allFileAssociations")));
	m_btnnoFileAssociations = static_cast<CButtonUI*>(m_pm->FindControl(_T("noFileAssociations")));
	m_tbndefaultFileAssociations = static_cast<CButtonUI*>(m_pm->FindControl(_T("defaultFileAssociations")));

	////////////////////////////////////////////////////文件关联开始
	CContainerUI* pContainer= static_cast<CContainerUI*>(m_pm->FindControl(_T("all_Associations")));

	bAlwaysAssociation=::GetPrivateProfileInt(_T("memory"),_T("alwaysFileAssociations"),-1,m_cfgPath);
	COptionUI* pOption= static_cast<COptionUI*>(m_pm->FindControl(_T("alwaysFileAssociations")));
	if(pOption!=NULL&&(bAlwaysAssociation==-1||bAlwaysAssociation==1))  pOption->  Selected(TRUE);

	BOOL bFileAssociations = FALSE;

	if(pContainer!=NULL&&m_playerSet!=NULL)
	{
		for(int i = 0; i < pContainer->GetCount(); ++i)
		{
			CControlUI* pControl = pContainer->GetItemAt(i);

			LPCTSTR strClass=pControl->GetClass();

			if(_tcscmp(strClass,_T("HorizontalLayoutUI"))==0)
			{	
				CContainerUI* pContainerSun= static_cast<CContainerUI*>(pControl);

				if(pContainerSun->GetCount()>0)
				{
					for(int i = 0; i < pContainerSun->GetCount(); ++i)	
					{
						CControlUI* pControl = pContainerSun->GetItemAt(i);				
						LPCTSTR strClass=pControl->GetClass();

						if(_tcscmp(strClass,_T("OptionUI"))==0)
						{
							COptionUI* pOption= static_cast<COptionUI*>(pControl);

							int iSelected=::GetPrivateProfileInt(_T("memory"),pOption->GetName(),-1,m_cfgPath);

							CString strExt,m_strIco=m_strIcoPath;
							strExt.Format(_T(".%s"),pOption->GetName());

							m_strIco.Append(pOption->GetName());							
							m_strIco.Append(_T(".ico"));

							if(iSelected==-1||iSelected==1)	
							{
								pOption->Selected(TRUE);	

								if(iSelected==1&&bAlwaysAssociation==1&&!IsRegistered(strExt,m_strExePath,m_strIco))
								{
									m_strIco.MakeLower();
									RegisterExt(strExt,m_strExePath,m_strIco);
									bFileAssociations=TRUE;
								}	
							}
							else
							{
								if(bAlwaysAssociation==1&&IsRegistered(strExt,m_strExePath,m_strIco))
								{
									DelRegister(strExt);
								}						
							}
						}
					}					
				}
			}
		}		
	}

	if(bAlwaysAssociation==1&&bFileAssociations)	SHChangeNotify(SHCNE_ASSOCCHANGED,SHCNF_IDLIST,0,0);
	/////////////////////////////////文件关联结束

}

void CSetDialog::Notify(TNotifyUI& msg)
{
	if( msg.sType == MST_WINDOWINIT ) 
	{
		OnPrepare();
	}
	else if(msg.sType == MST_VALUECHANGED)
	{
		Settings *pCfg = new Settings;
		pCfg->gammaCorrection = pCfg->gammaCorrectionDef;
		pCfg->hue = hue->GetValue()-180;
		pCfg->lumGain = contrast->GetValue();
		pCfg->lumOffset = pbrightness->GetValue()-256;
		pCfg->saturation = saturation->GetValue();
		::PostMessage(::GetParent(GetHWND()),MSG_MENUS,MM_PICCHANGE,(LPARAM)pCfg);
	}
	else if(msg.sType == MST_VALUECHANGING)
	{
		Settings *pCfg = new Settings;
		pCfg->gammaCorrection = pCfg->gammaCorrectionDef;
		pCfg->hue = hue->GetValue()-180;
		pCfg->lumGain = contrast->GetValue();
		pCfg->lumOffset = pbrightness->GetValue()-256;
		pCfg->saturation = saturation->GetValue();
		::PostMessage(::GetParent(GetHWND()),MSG_MENUS,MM_PICCHANGE,(LPARAM)pCfg);
	}
	else if (msg.sType == MST_CLICK)
	{
		if (msg.pSender == m_btnClose)
		{
			hue->SetValue(oldhue);
			contrast->SetValue(oldcontrast);
			pbrightness->SetValue(oldbrightness);
			saturation->SetValue(oldsaturation);
			m_pm->SendNotify(NULL,MST_VALUECHANGED);
			ShowWindow(false,false);
		}
		else if(msg.pSender == m_btnChangePath)
		{
			CStdString path = BrowseFolder(m_hWnd,_T("选择文件夹"));
			if(!path.IsEmpty())
			{
				if(path.GetAt(path.GetLength()-1)!='\\')
					path+=_T("\\");
				CEditUI *pEdit = static_cast<CEditUI*>(m_pm->FindControl(_T("pathedit"))); 
				pEdit->SetText(path);
			}
		}
		else if(msg.pSender == m_btnChangePicpath)
		{
			CStdString path = BrowseFolder(m_hWnd,_T("选择图片文件夹"));
			if(!path.IsEmpty())
			{
				if(path.GetAt(path.GetLength()-1)!='\\')
					path+=_T("\\");
				CEditUI *pEdit = static_cast<CEditUI*>(m_pm->FindControl(_T("pathedit2"))); 
				pEdit->SetText(path);
			}
		}
		else if(msg.pSender == suavity)
		{
			pbrightness->SetValue(235);
			saturation->SetValue(60);
			contrast->SetValue(128);
			hue->SetValue(180);
			m_pm->SendNotify(NULL,MST_VALUECHANGED);
		}
		else if(msg.pSender == bright)
		{
			pbrightness->SetValue(300);
			contrast->SetValue(135);
			saturation->SetValue(64);
			hue->SetValue(180);
			m_pm->SendNotify(NULL,MST_VALUECHANGED);
		}
		else if(msg.pSender == restore)
		{
			Settings *pCfg = new Settings;
			pbrightness->SetValue(256);
			saturation->SetValue(64);
			contrast->SetValue(128);
			hue->SetValue(180);
			m_pm->SendNotify(NULL,MST_VALUECHANGED);
		}
		else if(msg.pSender == m_btnallFileAssociations)
		{
			CContainerUI* pContainer= static_cast<CContainerUI*>(m_pm->FindControl(_T("all_Associations")));

			if(pContainer==NULL) return;

			for(int i = 0; i < pContainer->GetCount(); ++i)
			{
				CControlUI* pControl = pContainer->GetItemAt(i);

				LPCTSTR strClass=pControl->GetClass();

				if(_tcscmp(strClass,_T("HorizontalLayoutUI"))==0)
				{	
					CContainerUI* pContainerSun= static_cast<CContainerUI*>(pControl);

					if(pContainerSun->GetCount()>0)
					{
						for(int i = 0; i < pContainerSun->GetCount(); ++i)	
						{
							CControlUI* pControl = pContainerSun->GetItemAt(i);				
							LPCTSTR strClass=pControl->GetClass();

							if(_tcscmp(strClass,_T("OptionUI"))==0)
							{
								COptionUI* pOption= static_cast<COptionUI*>(pControl);	

								if(!pOption->IsSelected())
								{
									pOption->Selected(TRUE);
								}
							}
						}					
					}

				}
			}

		}
		else if(msg.pSender == m_btnnoFileAssociations)
		{
			CContainerUI* pContainer= static_cast<CContainerUI*>(m_pm->FindControl(_T("all_Associations")));

			if(pContainer==NULL) return;

			for(int i = 0; i < pContainer->GetCount(); ++i)
			{
				CControlUI* pControl = pContainer->GetItemAt(i);

				LPCTSTR strClass=pControl->GetClass();

				if(_tcscmp(strClass,_T("HorizontalLayoutUI"))==0)
				{	
					CContainerUI* pContainerSun= static_cast<CContainerUI*>(pControl);

					if(pContainerSun->GetCount()>0)
					{
						for(int i = 0; i < pContainerSun->GetCount(); ++i)	
						{
							CControlUI* pControl = pContainerSun->GetItemAt(i);				
							LPCTSTR strClass=pControl->GetClass();

							if(_tcscmp(strClass,_T("OptionUI"))==0)
							{
								COptionUI* pOption= static_cast<COptionUI*>(pControl);	

								if(pOption->IsSelected())
								{
									pOption->Selected(FALSE);
								}
							}
						}					
					}

				}
			}
		}
		else if(msg.pSender == m_tbndefaultFileAssociations)
		{
			CContainerUI* pContainer= static_cast<CContainerUI*>(m_pm->FindControl(_T("all_Associations")));

			if(pContainer==NULL) return;

			for(int i = 0; i < pContainer->GetCount(); ++i)
			{
				CControlUI* pControl = pContainer->GetItemAt(i);

				LPCTSTR strClass=pControl->GetClass();

				if(_tcscmp(strClass,_T("HorizontalLayoutUI"))==0)
				{	
					CContainerUI* pContainerSun= static_cast<CContainerUI*>(pControl);

					if(pContainerSun->GetCount()>0)
					{
						for(int i = 0; i < pContainerSun->GetCount(); ++i)	
						{
							CControlUI* pControl = pContainerSun->GetItemAt(i);				
							LPCTSTR strClass=pControl->GetClass();

							if(_tcscmp(strClass,_T("OptionUI"))==0)
							{
								COptionUI* pOption= static_cast<COptionUI*>(pControl);	

								if(pOption&&!pOption->IsSelected())
								{
									pOption->Selected(TRUE);
								}						
							}
						}					
					}

				}
			}
		}
		else if(msg.pSender == m_btnSure)
		{
			COptionUI* pOpt = static_cast<COptionUI*>(m_pm->FindControl(_T("minimize")));
			if(pOpt->IsSelected()) ::WritePrivateProfileString(_T("memory"),_T("close"),_T("0"),m_cfgPath);
			else ::WritePrivateProfileString(_T("memory"),_T("close"),_T("1"),m_cfgPath);//关闭还是最小化

			pOpt = static_cast<COptionUI*>(m_pm->FindControl(_T("playtop")));
			if(pOpt->IsSelected()) ::WritePrivateProfileString(_T("memory"),_T("top"),_T("1"),m_cfgPath);
			else
			{
				pOpt = static_cast<COptionUI*>(m_pm->FindControl(_T("top")));
				if(pOpt->IsSelected()) ::WritePrivateProfileString(_T("memory"),_T("top"),_T("2"),m_cfgPath);
				else ::WritePrivateProfileString(_T("memory"),_T("top"),_T("3"),m_cfgPath);
			}//置顶方式

			pOpt = static_cast<COptionUI*>(m_pm->FindControl(_T("autorun")));
			if(pOpt->IsSelected()) ::WritePrivateProfileString(_T("memory"),_T("autorun"),_T("1"),m_cfgPath);
			else ::WritePrivateProfileString(_T("memory"),_T("autorun"),_T("0"),m_cfgPath);//是否自动运行

			pOpt = static_cast<COptionUI*>(m_pm->FindControl(_T("qtacc")));
			if(pOpt->IsSelected()) ::WritePrivateProfileString(_T("memory"),_T("qtacc"),_T("1"),m_cfgPath);
			else ::WritePrivateProfileString(_T("memory"),_T("qtacc"),_T("0"),m_cfgPath);//是否自动运行

			pOpt = static_cast<COptionUI*>(m_pm->FindControl(_T("prim")));
			if(pOpt->IsSelected()) ::WritePrivateProfileString(_T("memory"),_T("FRAMERATIO"),_T("4"),m_cfgPath);
			else
			{
				pOpt = static_cast<COptionUI*>(m_pm->FindControl(_T("ftot")));
				if(pOpt->IsSelected()) ::WritePrivateProfileString(_T("memory"),_T("FRAMERATIO"),_T("6"),m_cfgPath);
				else 
				{
					pOpt = static_cast<COptionUI*>(m_pm->FindControl(_T("stton")));
					if(pOpt->IsSelected()) ::WritePrivateProfileString(_T("memory"),_T("FRAMERATIO"),_T("8"),m_cfgPath);
					else ::WritePrivateProfileString(_T("memory"),_T("FRAMERATIO"),_T("5"),m_cfgPath);
				}
			}//视频框架

			CComboUI* pCombo = static_cast<CComboUI*>(m_pm->FindControl(_T("cbrender")));
			int index =	pCombo->GetCurSel();
			if(index == 0 ) ::WritePrivateProfileString(_T("memory"),_T("renderer"),_T("2"),m_cfgPath);
			else if(index == 1 ) ::WritePrivateProfileString(_T("memory"),_T("renderer"),_T("3"),m_cfgPath);
			else if(index == 2 ) ::WritePrivateProfileString(_T("memory"),_T("renderer"),_T("4"),m_cfgPath);//渲染模式

			pOpt = static_cast<COptionUI*>(m_pm->FindControl(_T("click")));
			if(pOpt->IsSelected()) ::WritePrivateProfileString(_T("memory"),_T("clickpause"),_T("1"),m_cfgPath);
			else ::WritePrivateProfileString(_T("memory"),_T("clickpause"),_T("0"),m_cfgPath);//单击播放窗口暂停

			pOpt = static_cast<COptionUI*>(m_pm->FindControl(_T("hidepause"))); 
			if(pOpt->IsSelected()) ::WritePrivateProfileString(_T("memory"),_T("hidepause"),_T("1"),m_cfgPath);
			else ::WritePrivateProfileString(_T("memory"),_T("hidepause"),_T("0"),m_cfgPath);//隐藏窗口时是否暂停

			pOpt = static_cast<COptionUI*>(m_pm->FindControl(_T("remstation"))); 
			if(pOpt->IsSelected()) ::WritePrivateProfileString(_T("memory"),_T("remember"),_T("1"),m_cfgPath);
			else ::WritePrivateProfileString(_T("memory"),_T("remember"),_T("0"),m_cfgPath);//记录播放位置

			index = static_cast<CComboUI*>(m_pm->FindControl(_T("hour")))->GetCurSel();//小时
			CStdString d;
			d.Format(_T("%d"),index);
			::WritePrivateProfileString(_T("time"),_T("hour"),d,m_cfgPath);
			index = static_cast<CComboUI*>(m_pm->FindControl(_T("minute")))->GetCurSel();//分钟
			d.Format(_T("%d"),index);
			WritePrivateProfileString(_T("time"),_T("minute"),d,m_cfgPath);

			pOpt = static_cast<COptionUI*>(m_pm->FindControl(_T("enddown")));
			if(pOpt->IsSelected())//播放完成后关机
				::WritePrivateProfileString(_T("memory"),_T("down"),_T("1"),m_cfgPath);
			else 
			{
				pOpt = static_cast<COptionUI*>(m_pm->FindControl(_T("autoshotdown")));
				if(pOpt->IsSelected())//定时
					::WritePrivateProfileString(_T("memory"),_T("down"),_T("2"),m_cfgPath);
				else
				{
					pOpt = static_cast<COptionUI*>(m_pm->FindControl(_T("autooff")));
					if(pOpt->IsSelected())
						::WritePrivateProfileString(_T("memory"),_T("down"),_T("3"),m_cfgPath);
				}
			}

			pOpt = static_cast<COptionUI*>(m_pm->FindControl(_T("delstartup")));
			if(pOpt->IsSelected())
				::WritePrivateProfileString(_T("memory"),_T("delstartup"),_T("1"),m_cfgPath);
			else
				::WritePrivateProfileString(_T("memory"),_T("delstartup"),_T("0"),m_cfgPath);
			//自动删除

			pOpt = static_cast<COptionUI*>(m_pm->FindControl(_T("always")));
			if(pOpt->IsSelected())
			{
				CString a;
				::WritePrivateProfileString(_T("memory"),_T("always"),_T("1"),m_cfgPath);
				a.Format(_T("%d"),pbrightness->GetValue());
				::WritePrivateProfileString(_T("cfg"),_T("brightness"),a,m_cfgPath);
				a.Format(_T("%d"),saturation->GetValue());
				::WritePrivateProfileString(_T("cfg"),_T("saturation"),a,m_cfgPath);
				a.Format(_T("%d"),contrast->GetValue());
				::WritePrivateProfileString(_T("cfg"),_T("contrast"),a,m_cfgPath);
				a.Format(_T("%d"),hue->GetValue());
				::WritePrivateProfileString(_T("cfg"),_T("hue"),a,m_cfgPath);
			}
			else
				::WritePrivateProfileString(_T("memory"),_T("always"),_T("0"),m_cfgPath);
			//自动删除

			CEditUI *pEdit = static_cast<CEditUI*>(m_pm->FindControl(_T("pathedit"))); 
			::WritePrivateProfileString(_T("QVODTERMINAL"),_T("mediapath"),pEdit->GetText(),m_qvodiniPath);

			pEdit = static_cast<CEditUI*>(m_pm->FindControl(_T("delfile"))); 
			CStdString num = pEdit->GetText();
			int per = ::_wtoi(num);
			if(per<=0 || per>99) per = 0;
			if(per == 0) 
				::WritePrivateProfileString(_T("QVODTERMINAL"),_T("EnableDelTask"),_T("0"),m_qvodiniPath);
			else
			{
				::WritePrivateProfileString(_T("QVODTERMINAL"),_T("EnableDelTask"),_T("1"),m_qvodiniPath);
				num.Empty();
				num.Format(_T("%d"),per);
				::WritePrivateProfileString(_T("QVODTERMINAL"),_T("MinValue"),num,m_qvodiniPath);
			}

			pEdit = static_cast<CEditUI*>(m_pm->FindControl(_T("maxedit")));
			num.Empty();
			num = pEdit->GetText();
			per = ::_wtoi(num);
			if( per<2 ) num=_T("2");
			if( per>10) num=_T("10");
			::WritePrivateProfileString(_T("memory"),_T("maxedit"),num,m_cfgPath);

			pEdit = static_cast<CEditUI*>(m_pm->FindControl(_T("pathedit2")));
			::WritePrivateProfileString(_T("memory"),_T("picpath"),pEdit->GetText(),m_cfgPath);

			////////////////////////////////////////////////////文件关联开始
			CContainerUI* pContainer= static_cast<CContainerUI*>(m_pm->FindControl(_T("all_Associations")));

			COptionUI* pOption= static_cast<COptionUI*>(m_pm->FindControl(_T("alwaysFileAssociations")));

			if(pOption->IsSelected()) ::WritePrivateProfileString(_T("memory"),_T("alwaysFileAssociations"),_T("1"),m_cfgPath);
			else  ::WritePrivateProfileString(_T("memory"),_T("alwaysFileAssociations"),_T("0"),m_cfgPath);

			BOOL bFileAssociations=FALSE;
			if(pContainer!=NULL&&m_playerSet!=NULL)
			{
				for(int i = 0; i < pContainer->GetCount(); ++i)
				{
					CControlUI* pControl = pContainer->GetItemAt(i);

					LPCTSTR strClass=pControl->GetClass();

					if(_tcscmp(strClass,_T("HorizontalLayoutUI"))==0)
					{	
						CContainerUI* pContainerSun= static_cast<CContainerUI*>(pControl);

						if(pContainerSun->GetCount()>0)
						{
							for(int i = 0; i < pContainerSun->GetCount(); ++i)	
							{
								CControlUI* pControl = pContainerSun->GetItemAt(i);				
								LPCTSTR strClass=pControl->GetClass();

								if(_tcscmp(strClass,_T("OptionUI"))==0)
								{
									COptionUI* pOption= static_cast<COptionUI*>(pControl);

									CString m_strIco=m_strIcoPath;
									CString strExt;
									strExt.Format(_T(".%s"),pOption->GetName());

									m_strIco.Append(pOption->GetName());				
									m_strIco.Append(_T(".ico"));

									if(pOption->IsSelected())
									{
										::WritePrivateProfileString(_T("memory"),pOption->GetName(),_T("1"),m_cfgPath);	
										if(!IsRegistered(strExt,m_strExePath,m_strIco)) 
										{
											m_strIco.MakeLower();
											RegisterExt(strExt,m_strExePath,m_strIco);
											bFileAssociations = TRUE;
										}
									}
									else
									{
										::WritePrivateProfileString(_T("memory"),pOption->GetName(),_T("0"),m_cfgPath);
										if(IsRegistered(strExt,m_strExePath,m_strIco))
										{
											DelRegister(strExt);
										}
									}	
								}
							}					
						}

					}
				}			

			}

			if(bFileAssociations) SHChangeNotify(SHCNE_ASSOCCHANGED,SHCNF_IDLIST,0,0);
			/////////////////////////////////文件关联结束


			//finaly
			ShowWindow(false,false);
			::SendMessage(::GetParent(m_hWnd),MSG_MENUS,MM_CHANGESET,0);
		}
		else if(msg.pSender == m_btnGiveup)
		{
			hue->SetValue(oldhue);
			contrast->SetValue(oldcontrast);
			pbrightness->SetValue(oldbrightness);
			saturation->SetValue(oldsaturation);
			m_pm->SendNotify(NULL,MST_VALUECHANGED);
			ShowWindow(false,false);
		}
	}
	else if(msg.sType==MST_SELECTCHANGED)//顶部按钮选择~~
	{
		static BOOL i=0,j=0;
		CStdString name = msg.pSender->GetName();
		CTabLayoutUI* pControl = static_cast<CTabLayoutUI*>(m_pm->FindControl(_T("switch")));
		if(name==_T("normalset"))
		{
			pControl->SelectItem(0);
		}
		else if(name==_T("playset"))
		{
			pControl->SelectItem(1);
		}
		else if(name==_T("imageset"))
		{
			pControl->SelectItem(2);
		}
		else if(name==_T("FileAssociations"))	
		{
			pControl->SelectItem(3);
		}
		else if(name==_T("pathset"))
		{
			pControl->SelectItem(4);
		}
		else if(name==_T("timeauto"))
		{
			pControl->SelectItem(5);
		}
		else if(name==_T("hot"))
		{
			pControl->SelectItem(6);
		}
		else if(name==_T("aboutus"))
		{
			pControl->SelectItem(7);
		}
	}
	else if(msg.sType == MST_LINK)
	{
		if(msg.pSender == m_pm->FindControl(_T("eee")))
		{
			CTextUI* pText = static_cast<CTextUI*>(msg.pSender);
			ShellExecute(NULL,_T("open"),*pText->GetLinkContent(msg.wParam),NULL,NULL,SW_SHOWNORMAL);
		}
	}
}

void CSetDialog::ReadIni()
{
	BOOL b = ::GetPrivateProfileInt(_T("memory"),_T("close"),0,m_cfgPath);
	if(!b) static_cast<COptionUI*>(m_pm->FindControl(_T("minimize")))->Selected(TRUE);
	else static_cast<COptionUI*>(m_pm->FindControl(_T("exit")))->Selected(TRUE);//设置关闭后

	int i = ::GetPrivateProfileInt(_T("memory"),_T("top"),3,m_cfgPath);
	if(i == 1) static_cast<COptionUI*>(m_pm->FindControl(_T("playtop")))->Selected(TRUE);
	else if(i==2) static_cast<COptionUI*>(m_pm->FindControl(_T("top")))->Selected(TRUE);
	else static_cast<COptionUI*>(m_pm->FindControl(_T("untop")))->Selected(TRUE);//设置置顶

	b = ::GetPrivateProfileInt(_T("memory"),_T("autorun"),0,m_cfgPath);
	static_cast<COptionUI*>(m_pm->FindControl(_T("autorun")))->Selected(b);

	b = ::GetPrivateProfileInt(_T("memory"),_T("qtacc"),0,m_cfgPath);
	static_cast<COptionUI*>(m_pm->FindControl(_T("qtacc")))->Selected(b);//设置是否退出acc

	i = ::GetPrivateProfileInt(_T("memory"),_T("FRAMERATIO"),4,m_cfgPath);
	if(4==i) static_cast<COptionUI*>(m_pm->FindControl(_T("prim")))->Selected(TRUE);
	else if(6==i) static_cast<COptionUI*>(m_pm->FindControl(_T("ftot")))->Selected(TRUE);
	else if(8==i) static_cast<COptionUI*>(m_pm->FindControl(_T("stton")))->Selected(TRUE);
	else if(5==i) static_cast<COptionUI*>(m_pm->FindControl(_T("full")))->Selected(TRUE);//视频框架

	int renderertype = ::GetPrivateProfileInt(_T("memory"),_T("renderer"),VMR7,m_cfgPath);
	if(renderertype == VMR9) i = 1;
	else if(renderertype == EVR) i = 2;
	else i=0;
	static_cast<CComboUI*>(m_pm->FindControl(_T("cbrender")))->SelectItem(i);//渲染模式

	static_cast<COptionUI*>(m_pm->FindControl(_T("click")))->Selected(::GetPrivateProfileInt(_T("memory"),_T("clickpause"),1,m_cfgPath));
	//设置单击暂停

	static_cast<COptionUI*>(m_pm->FindControl(_T("hidepause")))->Selected(::GetPrivateProfileInt(_T("memory"),_T("hidepause"),0,m_cfgPath));
	//隐藏时暂停

	static_cast<COptionUI*>(m_pm->FindControl(_T("remstation")))->Selected(::GetPrivateProfileInt(_T("memory"),_T("remember"),1,m_cfgPath)); 
	//保存播放位置

	i = ::GetPrivateProfileInt(_T("time"),_T("hour"),0,m_cfgPath);
	static_cast<CComboUI*>(m_pm->FindControl(_T("hour")))->SelectItem(i);//小时
	i = ::GetPrivateProfileInt(_T("time"),_T("minute"),0,m_cfgPath);
	static_cast<CComboUI*>(m_pm->FindControl(_T("minute")))->SelectItem(i);

	i = ::GetPrivateProfileInt(_T("memory"),_T("down"),3,m_cfgPath);
	if(i == 1)
		static_cast<COptionUI*>(m_pm->FindControl(_T("enddown")))->Selected(TRUE);
	else if(i==2)
		static_cast<COptionUI*>(m_pm->FindControl(_T("autoshotdown")))->Selected(TRUE);
	else if(i==3)
		static_cast<COptionUI*>(m_pm->FindControl(_T("autooff")))->Selected(TRUE);//自动关机

	static_cast<COptionUI*>(m_pm->FindControl(_T("delstartup")))->Selected(::GetPrivateProfileInt(_T("memory"),_T("delstartup"),1,m_cfgPath));
	//自动删除

	static_cast<COptionUI*>(m_pm->FindControl(_T("always")))->Selected(::GetPrivateProfileInt(_T("memory"),_T("always"),0,m_cfgPath));
	//保持画面

	CEditUI *pEdit = static_cast<CEditUI*>(m_pm->FindControl(_T("pathedit"))); 
	TCHAR mediapath[MAX_PATH]={0};
	::GetPrivateProfileString(_T("QVODTERMINAL"),_T("mediapath"),pEdit->GetText(),mediapath,MAX_PATH,m_qvodiniPath);
	pEdit->SetText(mediapath);

	pEdit = static_cast<CEditUI*>(m_pm->FindControl(_T("maxedit")));
	::GetPrivateProfileString(_T("memory"),_T("maxedit"),_T("5"),mediapath,MAX_PATH,m_cfgPath);
	pEdit->SetText(mediapath);

	b = ::GetPrivateProfileInt(_T("QVODTERMINAL"),_T("EnableDelTask"),0,m_qvodiniPath);
	pEdit = static_cast<CEditUI*>(m_pm->FindControl(_T("delfile"))); 
	if(0==b)
		pEdit->SetText(_T("0"));
	else if(1==b)
	{
		i = ::GetPrivateProfileInt(_T("QVODTERMINAL"),_T("MinValue"),5,m_qvodiniPath);
		if(i<=0 || i>99)
			i = 0;
		CStdString per;
		per.Format(_T("%d"),i);
		pEdit->SetText(per);
	}
	oldhue = hue->GetValue();
	oldcontrast = contrast->GetValue();
	oldbrightness = pbrightness->GetValue();
	oldsaturation = saturation->GetValue();

	TCHAR picpath[MAX_PATH]={0};
	pEdit = static_cast<CEditUI*>(m_pm->FindControl(_T("pathedit2")));
	::GetPrivateProfileString(_T("memory"),_T("picpath"),m_strDesttop,picpath,MAX_PATH,m_cfgPath);
	pEdit->SetText(picpath);
}

LRESULT CSetDialog::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	RECT rcClient;
	::GetClientRect(*this, &rcClient);
	::SetWindowPos(*this, NULL, rcClient.left, rcClient.top, rcClient.right - rcClient.left, \
		rcClient.bottom - rcClient.top, SWP_FRAMECHANGED);

	m_pm->Init(m_hWnd);
	CDialogBuilder builder;
	CControlUI* pRoot = builder.Create((LPCTSTR)skn,_T("dialog.xml"), (UINT)0, NULL, m_pm);
	ASSERT(pRoot && "Failed to parse xml");
	m_pm->AttachDialog(pRoot);
	m_pm->AddNotifier(this);

	SIZE szInitWindowSize = m_pm->GetInitSize();
	if( szInitWindowSize.cx != 0 ) 
	{
		::SetWindowPos(*this, NULL, 0, 0, szInitWindowSize.cx, szInitWindowSize.cy, SWP_NOZORDER | SWP_NOMOVE);
	}

	Init();
	return 0;
}

LRESULT CSetDialog::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return (wParam == 0) ? TRUE : FALSE;
}

LRESULT CSetDialog::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CSetDialog::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CSetDialog::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
	::ScreenToClient(*this, &pt);

	RECT rcClient;
	::GetClientRect(*this, &rcClient);

	RECT rcCaption = m_pm->GetCaptionRect();
	if( pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
		&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom ) 
	{
		CControlUI* pControl = static_cast<CControlUI*>(m_pm->FindControl(pt));
		if( pControl && _tcscmp(pControl->GetClass(), _T("ButtonUI")) != 0 )
		{
			return HTCAPTION;
		}
	}

	return HTCLIENT;
}

LRESULT CSetDialog::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SIZE szRoundCorner = m_pm->GetRoundCorner();
	if (this == NULL)
	{
		return 0;
	}
	if( !::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0) ) 
	{
		RECT rcClient;
		::GetClientRect(*this, &rcClient);
		//RECT rc = { rcClient.left, rcClient.top + szRoundCorner.cx, rcClient.right, rcClient.bottom };
		//HRGN hRgn1 = ::CreateRectRgnIndirect( &rc );
		//HRGN hRgn2 = ::CreateRoundRectRgn(rcClient.left, rcClient.top, rcClient.right + 1, rcClient.bottom - szRoundCorner.cx, szRoundCorner.cx, szRoundCorner.cy);
		HRGN hRgn2 = ::CreateRoundRectRgn(rcClient.left, rcClient.top, rcClient.right, rcClient.bottom, szRoundCorner.cx, szRoundCorner.cy);
		//::CombineRgn( hRgn1, hRgn1, hRgn2, RGN_OR );
		::SetWindowRgn(*this, hRgn2, TRUE);
		//::DeleteObject(hRgn1);
		::DeleteObject(hRgn2);
	}
	bHandled = FALSE;
	return 0;
}

LRESULT CSetDialog::OnMoveWindow(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	RECT rtClient;
	POINTS pt	= MAKEPOINTS(lParam);
	int x = (int)(short) LOWORD(lParam);
	int y = (int)(short) HIWORD(lParam);
	::GetWindowRect(m_hWnd,&rtClient);
	return MoveWindow((int)(short) LOWORD(lParam),(int)(short) HIWORD(lParam),rtClient.right - rtClient.left,rtClient.bottom - rtClient.top,FALSE);;
}

LRESULT CSetDialog::OnRegFile(UINT uMsg, WPARAM wParam, LPARAM lParam,BOOL& bHandled)
{
	////////////////////////////////////////////////////文件关联开始
	CContainerUI* pContainer= static_cast<CContainerUI*>(m_pm->FindControl(_T("all_Associations")));

	BOOL bFileAssociations = FALSE;

	if(pContainer!=NULL&&m_playerSet!=NULL)
	{
		for(int i = 0; i < pContainer->GetCount(); ++i)
		{
			CControlUI* pControl = pContainer->GetItemAt(i);

			LPCTSTR strClass=pControl->GetClass();

			if(_tcscmp(strClass,_T("HorizontalLayoutUI"))==0)
			{	
				CContainerUI* pContainerSun= static_cast<CContainerUI*>(pControl);

				if(pContainerSun->GetCount()>0)
				{
					for(int i = 0; i < pContainerSun->GetCount(); ++i)	
					{
						CControlUI* pControl = pContainerSun->GetItemAt(i);				
						LPCTSTR strClass=pControl->GetClass();

						if(_tcscmp(strClass,_T("OptionUI"))==0)
						{
							COptionUI* pOption= static_cast<COptionUI*>(pControl);

							int iSelected=::GetPrivateProfileInt(_T("memory"),pOption->GetName(),-1,m_cfgPath);

							CString strExt,m_strIco=m_strIcoPath;
							strExt.Format(_T(".%s"),pOption->GetName());

							m_strIco.Append(pOption->GetName());							
							m_strIco.Append(_T(".ico"));

							if(iSelected==-1||iSelected==1)	
							{
								pOption->Selected(TRUE);	

								if(!IsRegistered(strExt,m_strExePath,m_strIco))
								{
									m_strIco.MakeLower();
									RegisterExt(strExt,m_strExePath,m_strIco);
									bFileAssociations=TRUE;
								}	
							}
							else
							{
								if(IsRegistered(strExt,m_strExePath,m_strIco))
								{
									DelRegister(strExt);
								}						
							}
						}
					}					
				}
			}
		}		
	}

	if(bFileAssociations)	SHChangeNotify(SHCNE_ASSOCCHANGED,SHCNF_IDLIST,0,0);
	/////////////////////////////////文件关联结束

	bHandled = TRUE;
	return 0;
}

LRESULT CSetDialog::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
	if( wParam == SC_CLOSE ) 
	{
		::PostQuitMessage(0L);
		bHandled = TRUE;
		return 0;
	}
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	return lRes;
}

LRESULT CSetDialog::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch( uMsg ) 
	{
	case WM_CREATE:			lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
	case WM_NCACTIVATE:		lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
	case WM_NCCALCSIZE:		lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
	case WM_NCPAINT:		lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
	case WM_NCHITTEST:		lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
	case WM_SIZE:			lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
	case WM_MOVE:			lRes = OnMoveWindow(uMsg,wParam,lParam);break;
	case WM_REG_FILE:		lRes = OnRegFile(uMsg,wParam,lParam,bHandled);break;
	default:
		bHandled = FALSE;
	}
	if( bHandled ) 
		return lRes;

	if( m_pm->MessageHandler(uMsg, wParam, lParam, lRes) ) 
		return lRes;

	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

CStdString CSetDialog::BrowseFolder(HWND hWnd, LPCTSTR lpTitle)
{
	// 调用 SHBrowseForFolder 取得目录(文件夹)名称
	// 参数 hWnd: 父窗口句柄
	// 参数 lpTitle: 窗口标题
	TCHAR szPath[MAX_PATH]={0};
	BROWSEINFO m_bi;

	m_bi.ulFlags = BIF_RETURNONLYFSDIRS  | BIF_STATUSTEXT;
	m_bi.hwndOwner = hWnd;
	m_bi.pidlRoot = NULL;
	m_bi.lpszTitle = lpTitle;
	m_bi.lpfn = NULL;
	m_bi.lParam = NULL;
	m_bi.pszDisplayName = szPath;

	LPITEMIDLIST pidl = ::SHBrowseForFolder( &m_bi );
	if ( pidl )
	{
		if( !::SHGetPathFromIDList ( pidl,szPath ) )  szPath[0]=0;
		IMalloc * pMalloc = NULL;
		if ( SUCCEEDED ( ::SHGetMalloc( &pMalloc ) ) )  // 取得IMalloc分配器接口
		{
			pMalloc->Free( pidl );    // 释放内存
			pMalloc->Release();       // 释放接口
		}
	}
	return szPath;
}

////////////////////////////////////////////////////////////

bool CSetDialog::IsRegistered(CString ext,CString exe,CString icon)
{
	CString strProgID = _T("ttk") + ext;

	// The 2000/XP way
	CRegKey		key;
	TCHAR		buff[_MAX_PATH];
	ULONG		len = sizeof(buff)/sizeof(buff[0]);
	memset(buff, 0, sizeof(buff));

	if(ERROR_SUCCESS != key.Open(HKEY_CLASSES_ROOT, ext)) {
		return false;
	}

	if(ERROR_SUCCESS != key.QueryStringValue(NULL, buff, &len) && !CString(buff).Trim().IsEmpty()) {
		return false;
	}

	if(!(buff == strProgID))
		return false;

	len = sizeof(buff)/sizeof(buff[0]);
	memset(buff, 0, sizeof(buff));
	if(ERROR_SUCCESS != key.Open(HKEY_CLASSES_ROOT, strProgID + _T("\\shell\\open\\command"), KEY_READ)) 
		return false;
	else
	{
		CString strCommand = exe + _T(" \"%1\"");
		if (ERROR_SUCCESS != key.QueryStringValue(NULL, buff, &len) && !CString(buff).Trim().IsEmpty())
			return false;
		else
		{
			if(strCommand.CompareNoCase(CString(buff)) != 0)
				return false;
		}
	}

	len = sizeof(buff)/sizeof(buff[0]);
	memset(buff, 0, sizeof(buff));
	if(ERROR_SUCCESS != key.Open(HKEY_CURRENT_USER,_T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\")+ext, KEY_READ))
		return false;
	else
	{
		if(ERROR_SUCCESS != key.QueryStringValue(_T("Progid"), buff, &len) && !CString(buff).Trim().IsEmpty()) 
			return false;
		else
		{
			if(strProgID.CompareNoCase(CString(buff)) != 0)
				return false;
		}
	}

	len = sizeof(buff)/sizeof(buff[0]);
	memset(buff, 0, sizeof(buff));
	if(ERROR_SUCCESS != key.Open(HKEY_CLASSES_ROOT,strProgID + _T("\\DefaultIcon"), KEY_READ))
		return false;
	else
	{
		if (ERROR_SUCCESS != key.QueryStringValue(NULL, buff, &len) && !CString(buff).Trim().IsEmpty())
			return false;
		else
		{
			if(icon.CompareNoCase(CString(buff)) != 0)
				return false;
		}
	}

	return true;
}

bool CSetDialog::DelRegister(CString ext)
{
	CRegKey		key;
	CString strProgID = _T("ttk") + ext;

	TCHAR		buff[_MAX_PATH];
	ULONG		len = sizeof(buff)/sizeof(buff[0]);
	memset(buff, 0, sizeof(buff));

	if(ERROR_SUCCESS != key.Open(HKEY_CLASSES_ROOT, ext, KEY_ALL_ACCESS) )
	{
		return(false);
	}

	if(ERROR_SUCCESS == key.QueryStringValue(_T("ttkbak"),buff,&len) && !CString(buff).Trim().IsEmpty())
	{
		if(ERROR_SUCCESS != key.SetStringValue(NULL,CString(buff).Trim()))
			return false;
	}
	else
		return false;

	if(ERROR_SUCCESS != key.Open(HKEY_CURRENT_USER,_T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\")+ext, KEY_ALL_ACCESS)){
		return false;
	}

	len = sizeof(buff)/sizeof(buff[0]);
	memset(buff, 0, sizeof(buff));
	if(ERROR_SUCCESS == key.QueryStringValue(_T("ttkbakprogid"),buff,&len) && !CString(buff).Trim().IsEmpty())
	{
		if(ERROR_SUCCESS != key.SetStringValue(_T("Progid"),CString(buff).Trim()))
			return false;
	}

	return true;
}

bool CSetDialog::RegisterExt(CString ext,CString exe,CString icon)
{
	CRegKey		key;
	CString strProgID = _T("ttk") + ext;

	TCHAR		buff[_MAX_PATH];
	ULONG		len = sizeof(buff)/sizeof(buff[0]);
	memset(buff, 0, sizeof(buff));

	// Create ProgID for this file type
	if(ERROR_SUCCESS != key.Create(HKEY_CLASSES_ROOT, strProgID)) {
		return(false);
	}

	if(ERROR_SUCCESS != key.Create(HKEY_CLASSES_ROOT, strProgID + _T("\\DefaultIcon"))) {
		return(false);
	}

	if((ERROR_SUCCESS != key.SetStringValue(NULL, icon))) {
		return(false);
	}

	if(ERROR_SUCCESS != key.Create(HKEY_CLASSES_ROOT, ext) ){
		return(false);
	}

	if(ERROR_SUCCESS == key.QueryStringValue(NULL,buff,&len) && !CString(buff).Trim().IsEmpty())
	{
		if(strProgID.CompareNoCase(CString(buff).Trim()) != 0)
			key.SetStringValue(_T("ttkbak"),CString(buff).Trim());
	}

	if(ERROR_SUCCESS != key.SetStringValue(NULL, strProgID)) {
		return(false);
	}

	if(ERROR_SUCCESS != key.Create(HKEY_CLASSES_ROOT, strProgID + _T("\\shell"))) {
		return(false);
	}

	if(ERROR_SUCCESS != key.SetStringValue(NULL, _T("open"))) {
		return(false);
	}

	if(ERROR_SUCCESS != key.Create(HKEY_CLASSES_ROOT, strProgID + _T("\\shell\\open"))) {
		return(false);
	}

	if(ERROR_SUCCESS != key.Create(HKEY_CLASSES_ROOT, strProgID + _T("\\shell\\open\\command"))) {
		return(false);
	}

	if((ERROR_SUCCESS != key.SetStringValue(NULL, exe + _T(" \"%1\"")))) {
		return(false);
	}

	if(ERROR_SUCCESS != key.Create(HKEY_CURRENT_USER,_T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\")+ext)){
		return false;
	}

	len = sizeof(buff)/sizeof(buff[0]);
	memset(buff, 0, sizeof(buff));
	if(ERROR_SUCCESS == key.QueryStringValue(_T("Progid"),buff,&len) && !CString(buff).Trim().IsEmpty())
	{
		if(strProgID.CompareNoCase(CString(buff).Trim()) != 0)
			key.SetStringValue(_T("ttkbakprogid"),CString(buff).Trim());
	}

	if(ERROR_SUCCESS != key.SetStringValue(_T("Progid"), strProgID)) {
		return(false);
	}

	return(true);
}