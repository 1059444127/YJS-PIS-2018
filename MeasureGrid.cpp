// MeasureGrid.cpp: implementation of the CMeasureGrid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Pathology.h"
#include "Pathologyview.h"
#include "Pathologydoc.h"
#include "MeasureGrid.h"
#include "mainfrm.h"
#include "ImageView.h"
#include "ImageDockWnd.h"
#include "FieldOut.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CMeasureGrid,CUGCtrl)
//{{AFX_MSG_MAP(CMeasureGrid)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern CPathologyApp theApp;

CMeasureGrid::CMeasureGrid()
{

}

CMeasureGrid::~CMeasureGrid()
{

}

void CMeasureGrid::OnSetup()
{
	m_font.CreateFont(12,0,0,0,400,0,0,0,0,0,0,0,0,"MS Sans Serif");
	SetDefFont(&m_font);	

	//setup the grid
	SetNumberRows(0);
	SetNumberCols(4);
	SetSH_Width(25);// 0);
	SetSH_NumberCols(1);

	SetColWidth(0,150);
	SetColWidth(1,300);
	SetColWidth(2,150);
	SetColWidth(3,300);
	
	SetHighlightRow(TRUE);
	
	SetVScrollMode( UG_SCROLLTRACKING );
	SetHScrollMode( UG_SCROLLTRACKING );

	AddMenuItem(1000,"测量结果输出");
	EnableMenu(1);	
}

void CMeasureGrid::OnGetCell(int col,long row,CUGCell *cell)
{
	static char buf[_MAX_PATH];

	if( row == -1 )     //  Display table head information of pRecordSet_Define
	{
		if( col >= 0 )
		{
			switch ( col )
			{
			case 0:
				strcpy(buf,"矢量类别");
				break;
			case 1:
				strcpy(buf,"中心点");
				break;
			case 2:
				strcpy(buf,"长度");
				break;
			case 3:
				strcpy(buf,"面积");
				break;
			}
			cell->SetText(buf);
		}
		return;
	}
	if( col == -1 )
	{
		if( row >= 0 )
		{
			if( row == GetCurrentRow() )
				cell->SetCellType(UGCT_ARROW);
		}
		return;
	}

	CImageView *pView = NULL;
	if(theApp.GetView()->m_ImageDockWnd.GetSafeHwnd())
	{
		CWnd *pWnd = theApp.GetView()->m_ImageDockWnd.GetWindow(GW_CHILD);
		while( pWnd != NULL )
		{
			if( pWnd->IsKindOf(RUNTIME_CLASS(CImageView)) )
			{
				pView = (CImageView *)pWnd;
				break;
			}
			pWnd = pWnd->GetNextWindow(GW_HWNDNEXT);
		}
	}

	if(pView == NULL) return;

	CFODrawShape *pShape = NULL;
	POSITION pos = pView->GetCurrentModel()->GetFormObjects()->GetHeadPosition();
	int i = 0;
	while (pos != NULL) 
	{
		pShape = (CFODrawShape *)pView->GetCurrentModel()->GetFormObjects()->GetNext(pos);
		if(pShape == NULL || i >= row)	break;
		i++;
	}
	if( pShape == NULL || i != row )  return;
	
	UINT n_ShapeType = pShape->GetType();
	strcpy(buf," ");
	switch( col )
	{
	case 0:
		switch(n_ShapeType)
		{
		case FO_COMP_LINE:
			strcpy(buf,"线条");//CFOLineShape");
			break;
		case FO_COMP_ARROWLINE:
			strcpy(buf,"线条");//"CFOLineShape with an arrow");
			break;
		case FO_COMP_CROSSLINE:
			strcpy(buf,"线条");//"CFOLineShape with an arrow");
			break;
		case FO_COMP_ROUND:
			strcpy(buf,"CFOBezierLineShape");
			break;
		case FO_COMP_FREELINE:
			strcpy(buf,"自由曲线");//"CFOFreeLineShape");
			break;
		case FO_COMP_FREECLOSELINE:
			strcpy(buf,"自由闭合曲线");//"CFOPolygonShape");
			break;
		case FO_COMP_POLYLINE:
			strcpy(buf,"折线");//"CFOLineShape with multi points");
			break;
		case FO_COMP_POLYGON:
			strcpy(buf,"多边形");//"CFOPolygonShape");
			break;
		case FO_COMP_ELLIPSE:
			strcpy(buf,"椭圆");//"CFOEllipseShape");
			break;
		case FO_COMP_RECTANGLE:
			strcpy(buf,"矩形");//"CFORectShape");
			break;
		case FO_COMP_CLOSEBEZIER:
			strcpy(buf,"CFOCloseBezierShape");
			break;
		case FO_COMP_STATIC:
			strcpy(buf,"文字注记");//"CFOStaticShape");
			break;
		case FO_COMP_HYLINK:
			strcpy(buf,"超级联接");//"CFOHyLinkShape");
			break;
		case FO_COMP_IMAGE:
			strcpy(buf,"图像标记");//"CFOImageShape");
			break;
		case FO_COMP_COMPOSITE:
			strcpy(buf,"点");//"CFOCompositeShape");
			break;
		case FO_COMP_GROUP:
			strcpy(buf,"群组"); //"CFOGroupShape");
			break;
		default:
			strcpy(buf,"Unknown Shape Type");
		}
		break;
	case 1:
		switch(n_ShapeType)
		{
		case FO_COMP_LINE:
		case FO_COMP_ARROWLINE:
		case FO_COMP_CROSSLINE:
		case FO_COMP_ROUND:
		case FO_COMP_FREELINE:
		case FO_COMP_FREECLOSELINE:
		case FO_COMP_POLYLINE:
		case FO_COMP_POLYGON:
		case FO_COMP_CLOSEBEZIER:
		case FO_COMP_STATIC:
		case FO_COMP_HYLINK:
		case FO_COMP_IMAGE:
		case FO_COMP_ELLIPSE:
		case FO_COMP_RECTANGLE:
		case FO_COMP_COMPOSITE:
			{
				float n_CenterPoint_X = 0.0, n_CenterPoint_Y = 0.0;
				CRect rt = pShape->GetRect();
				LpToGeop( (rt.left + rt.right) / 2  , (rt.top + rt.bottom) / 2, n_CenterPoint_X, n_CenterPoint_Y);
				
				sprintf(buf,"( %.6f , %.6f )",n_CenterPoint_X, n_CenterPoint_Y);
			}
			break;
		}
		break;
	case 2:
		switch(n_ShapeType)
		{
		case FO_COMP_LINE:
		case FO_COMP_ARROWLINE:
		case FO_COMP_FREELINE:
		case FO_COMP_FREECLOSELINE:
		case FO_COMP_POLYGON:
			{
				float n_Lengh = 0.0, startx,starty,endx,endy;
				int spotcount = pShape->GetSpotCount();
				LPPOINT pt = pShape->GetPoints();
				for(int i = 0; i < ( spotcount - 1); i++)
				{
					LpToGeop( pt[i].x  , pt[i].y,   startx, starty);
					LpToGeop( pt[i+1].x, pt[i+1].y, endx,   endy);
					n_Lengh = (float)(n_Lengh + sqrt((endx - startx) * (endx - startx) + (endy - starty) * (endy - starty)));
				}
				
				if( n_ShapeType == FO_COMP_FREECLOSELINE || n_ShapeType == FO_COMP_POLYGON )
				{
					LpToGeop( pt[spotcount - 1].x  , pt[spotcount - 1].y,   startx, starty);
					LpToGeop( pt[0].x, pt[0].y, endx,   endy);
					n_Lengh = (float)(n_Lengh + sqrt((endx - startx) * (endx - startx) + (endy - starty) * (endy - starty)));
				}
				
				sprintf(buf,"%.6f",n_Lengh);
			}
			break;
		case FO_COMP_RECTANGLE:
			{
				CRect rt = pShape->GetRect();
				float n_Lengh = 0.0, startx,starty,endx,endy;
				
				LpToGeop( rt.left  , rt.top, startx, starty);
				LpToGeop( rt.right , rt.top, endx,   endy);
				n_Lengh = (float)(n_Lengh + sqrt((endx - startx) * (endx - startx) + (endy - starty) * (endy - starty)));
				
				LpToGeop( rt.left  , rt.bottom, startx, starty);
				LpToGeop( rt.right , rt.bottom, endx,   endy);
				n_Lengh = (float)(n_Lengh + sqrt((endx - startx) * (endx - startx) + (endy - starty) * (endy - starty)));
				
				LpToGeop( rt.left  , rt.top,    startx, starty);
				LpToGeop( rt.left  , rt.bottom, endx,   endy);
				n_Lengh = (float)(n_Lengh + sqrt((endx - startx) * (endx - startx) + (endy - starty) * (endy - starty)));
				
				LpToGeop( rt.right  , rt.top,    startx, starty);
				LpToGeop( rt.right  , rt.bottom, endx,   endy);
				n_Lengh = (float)(n_Lengh + sqrt((endx - startx) * (endx - startx) + (endy - starty) * (endy - starty)));
				
				sprintf(buf,"%.6f",n_Lengh);
			}
			break;
		case FO_COMP_ELLIPSE:
			{
				CRect rt = pShape->GetRect();
				float n_Lengh = 0.0, startx,starty,endx,endy;
				
				LpToGeop( rt.left  , rt.top, startx, starty);
				LpToGeop( rt.right , rt.top, endx,   endy);
				float a = (float)sqrt((endx - startx) * (endx - startx) + (endy - starty) * (endy - starty));
				
				LpToGeop( rt.left  , rt.top,    startx, starty);
				LpToGeop( rt.left  , rt.bottom, endx,   endy);
				float b = (float)sqrt((endx - startx) * (endx - startx) + (endy - starty) * (endy - starty));
				
				n_Lengh = (float)(3.1415926 * ( a + b ) / 2);
				
				sprintf(buf,"%.6f",n_Lengh);
			}
			break;
		}
		break;
	case 3:
		switch(n_ShapeType)
		{
		case FO_COMP_FREELINE:
		case FO_COMP_LINE:
			/*{
				float startx,starty,endx,endy;
				int spotcount = pShape->GetSpotCount();
				LPPOINT pt = pShape->GetPoints();
				
				LpToGeop( pDoc, pt[0].x  , pt[0].y,   startx, starty);
				LpToGeop( pDoc, pt[spotcount-1].x, pt[spotcount-1].y, endx,   endy);
				
				sprintf(buf,"起点: ( %.4f , %.4f ) ; 终点: ( %.4f , %.4f )",startx,starty,endx,endy);
			}*/
			break;
		case FO_COMP_FREECLOSELINE:
		case FO_COMP_POLYGON:
			{
				float area = 0.0, startx,starty,endx,endy;
				int spotcount = pShape->GetSpotCount();
				LPPOINT pt = pShape->GetPoints();
				for(int i = 0; i < ( spotcount - 1); i++)
				{
					startx = (float)pt[i].x;
					starty = (float)pt[i].y;
					endx   = (float)pt[i+1].x;
					endy   = (float)pt[i+1].y;
					
					area += ( (endx - startx) * (endy + starty) );
				}
				
				startx = (float)pt[spotcount-1].x;
				starty = (float)pt[spotcount-1].y;
				endx   = (float)pt[0].x;
				endy   = (float)pt[0].y;

				area += ( (endx - startx) * (endy + starty) );
				if( area <= 0 ) area = -area;
				area /= 2;
				
				LpToGeop(area, area);
				
				sprintf(buf,"%.6f",area);
			}
			break;
		case FO_COMP_RECTANGLE:
			{
				CRect rt = pShape->GetRect();
				float area = 0.0, startx,starty,endx,endy;
				
				LpToGeop( rt.left  , rt.top, startx, starty);
				LpToGeop( rt.right , rt.top, endx,   endy);
				float a = (float)sqrt((endx - startx) * (endx - startx) + (endy - starty) * (endy - starty));
				
				LpToGeop( rt.left  , rt.top,    startx, starty);
				LpToGeop( rt.left  , rt.bottom, endx,   endy);
				float b = (float)sqrt((endx - startx) * (endx - startx) + (endy - starty) * (endy - starty));
				
				area = a * b;
				
				sprintf(buf,"%.6f",area);
			}
			break;
		case FO_COMP_ELLIPSE:
			{
				CRect rt = pShape->GetRect();
				float area = 0.0, startx,starty,endx,endy;
				
				LpToGeop( rt.left  , rt.top, startx, starty);
				LpToGeop( rt.right , rt.top, endx,   endy);
				float a = (float)sqrt((endx - startx) * (endx - startx) + (endy - starty) * (endy - starty));
				
				LpToGeop( rt.left  , rt.top,    startx, starty);
				LpToGeop( rt.left  , rt.bottom, endx,   endy);
				float b = (float)sqrt((endx - startx) * (endx - startx) + (endy - starty) * (endy - starty));
				
				area = (float)(3.1415926 *  a * b / 4);
				
				sprintf(buf,"%.6f",area);
			}
			break;
		}
		break;
	}
	cell->SetText(buf);
	return;
}


void CMeasureGrid::LpToGeop(int xx,int yy, float &ppx, float &ppy) 
{
	// TODO: Add your command update UI handler code here
	int nCount = ((CMainFrame*)AfxGetMainWnd())->m_wndComboRulerMark.GetCount();
	int nIndex = ((CMainFrame*)AfxGetMainWnd())->m_wndComboRulerMark.GetCurSel();
	STANDARD_UNIT m_unit;
	m_unit.zoomlen = 0;
	if ( (nIndex != LB_ERR) && (nCount > 0) )
	{
		CFile file;
		CFileException fe;
		if (file.Open(theApp.m_Exe_Working_Dir + "system/rulermark.dat", CFile::modeRead | CFile::shareDenyWrite, &fe))
		{
			file.Seek(nIndex * sizeof(STANDARD_UNIT), CFile::begin);
			file.Read(&m_unit, sizeof(STANDARD_UNIT));
			file.Close();
		}
	}
    if( m_unit.zoomlen == 0 )
	{
		ppx = xx;
		ppy = yy;
	}
	else
	{
		ppx = xx * m_unit.value;
		ppy = yy * m_unit.value;
	}
}

void CMeasureGrid::LpToGeop(float invalue, float &outvalue)
{
	int nCount = ((CMainFrame*)AfxGetMainWnd())->m_wndComboRulerMark.GetCount();
	int nIndex = ((CMainFrame*)AfxGetMainWnd())->m_wndComboRulerMark.GetCurSel();
	STANDARD_UNIT m_unit;
	m_unit.zoomlen = 0;
	if ( (nIndex != LB_ERR) && (nCount > 0) )
	{
		CFile file;
		CFileException fe;
		if (file.Open(theApp.m_Exe_Working_Dir + "system/rulermark.dat", CFile::modeRead | CFile::shareDenyWrite, &fe))
		{
			file.Seek(nIndex * sizeof(STANDARD_UNIT), CFile::begin);
			file.Read(&m_unit, sizeof(STANDARD_UNIT));
			file.Close();
		}
	}
    if( m_unit.zoomlen == 0 )
	{
		outvalue = invalue;
	}
	else
	{
		outvalue = invalue * m_unit.value * m_unit.value;
	}
}

void CMeasureGrid::OnMenuCommand(int col,long row,int section,int item)
{
    if(col < 0 || col >= GetNumberCols()) return;
    if(row < 0 || row >= GetNumberRows()) return;

	CImageView *pView = NULL;
	if(theApp.GetView()->m_ImageDockWnd.GetSafeHwnd())
	{
		CWnd *pWnd = theApp.GetView()->m_ImageDockWnd.GetWindow(GW_CHILD);
		while( pWnd != NULL )
		{
			if( pWnd->IsKindOf(RUNTIME_CLASS(CImageView)) )
			{
				pView = (CImageView *)pWnd;
				break;
			}
			pWnd = pWnd->GetNextWindow(GW_HWNDNEXT);
		}
	}

	if(pView == NULL) return;

	if( item == 1000 )
	{
		CString GridContext;
		CUGCell  cell;
		GetCellIndirect(col , row, &cell);
		cell.GetText(&GridContext);

		STANDARD_UNIT m_unit;
		m_unit.value = 1.0;
		m_unit.zoomlen = 0;
		strcpy(m_unit.unitname,"像素");
		
		int nCount = ((CMainFrame*)AfxGetMainWnd())->m_wndComboRulerMark.GetCount();
		int nIndex = ((CMainFrame*)AfxGetMainWnd())->m_wndComboRulerMark.GetCurSel();
		if ( (nIndex != LB_ERR) && (nCount > 0) )
		{
			CFile file;
			CFileException fe;
			if (file.Open(theApp.m_Exe_Working_Dir + "rulermark.dat", CFile::modeRead | CFile::shareDenyWrite, &fe))
			{
				file.Seek(nIndex * sizeof(STANDARD_UNIT), CFile::begin);
				file.Read(&m_unit, sizeof(STANDARD_UNIT));
				file.Close();
			}
		}

		//======================================================================
		theApp.ConnectDatabase();
		
		CFieldOut mydlg;

		CStringArray FieldNameList;
		CUIntArray   FieldTypeList;
		
		try
		{
			SAString cmdstr = "Select TOP 1 * from " + theApp.TABLE_MAIN;
			if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
			{
				cmdstr = "Select * from " + theApp.TABLE_MAIN + " where ROWNUM<=1";
			}
			g_dbcommand.setCommandText(cmdstr);
			
			g_dbcommand.Execute();
			
			g_dbcommand.FetchNext();
			int FieldCount = g_dbcommand.FieldCount();
			for( int i = 1; i <= FieldCount; i++ )
			{
				CString str = g_dbcommand.Field(i).Name();
				if( str.CompareNoCase("medicalno") == 0)
				{
					continue;
				}
				SADataType_t tt = g_dbcommand.Field(i).FieldType();
				if(!( tt == SA_dtShort    || 
					  tt == SA_dtLong     || 
					  tt == SA_dtDouble   || 
					  tt == SA_dtString   ||
					  tt == SA_dtLongChar ||
					  tt == SA_dtCLob ))
				{
					continue;
				}

				FieldNameList.Add(g_dbcommand.Field(i).Name());
				FieldTypeList.Add(g_dbcommand.Field(i).FieldType());
				
				for( int k = 0; k < theApp.m_fnamelist.GetSize(); k++ )
				{
					if( str.Compare( theApp.m_fnamelist.GetAt( k ) ) == 0 )  break;
				}
				if( k < theApp.m_fnamelist.GetSize() )
				{
					mydlg.strlist.Add( theApp.m_fdescriptionlist.GetAt(k) );
				}
				else
				{
					mydlg.strlist.Add( str );
				}
			}
			
			g_dbconnection.Commit();
		}
		catch(SAException &x)
		{
			try
			{
				g_dbconnection.Rollback();
			}
			catch(SAException &)
			{
			}
			
			AfxMessageBox((const char*)x.ErrText());
		}
		
		if( mydlg.DoModal() == IDOK )
		{
			SADataType_t type = (SADataType_t)FieldTypeList.GetAt(mydlg.m_Index);
			SAString strvalue;
			if( type == SA_dtLongChar || type == SA_dtCLob )
			{
				try
				{
					SAString cmdstr = "Select " + FieldNameList.GetAt(mydlg.m_Index) + " from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
					
					g_dbcommand.setCommandText( cmdstr );
					g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
					
					g_dbcommand.Execute();
					
					if( g_dbcommand.FetchNext() )
					{
						switch( type ) {
						case SA_dtLongChar:
							strvalue = g_dbcommand.Field((SAString)FieldNameList.GetAt(mydlg.m_Index)).asLongChar();
							break;
						case SA_dtCLob:
							strvalue = g_dbcommand.Field((SAString)FieldNameList.GetAt(mydlg.m_Index)).asCLob();
							break;
						}
					}
					
					g_dbconnection.Commit();
				}
				catch(SAException &x)
				{
					try
					{
						g_dbconnection.Rollback();
					}
					catch(SAException &)
					{
					}
					AfxMessageBox((const char*)x.ErrText());
				}

			}

			strvalue += " ";
			strvalue += GridContext;
			strvalue += " ";
			strvalue += m_unit.unitname;
			
			try
			{
				SAString cmdstr = "Update " + theApp.TABLE_MAIN + " set " + FieldNameList.GetAt(mydlg.m_Index) + " = :" + FieldNameList.GetAt(mydlg.m_Index) + " where medicalno = :medicalno";
				g_dbcommand.setCommandText( cmdstr );

				g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
				
				switch( (SADataType_t)FieldTypeList.GetAt(mydlg.m_Index) ) {
				case SA_dtShort:
					g_dbcommand.Param(( SAString )FieldNameList.GetAt(mydlg.m_Index)).setAsShort() = atoi(GridContext);
					break;
				case SA_dtLong:
					g_dbcommand.Param(( SAString )FieldNameList.GetAt(mydlg.m_Index)).setAsLong()  = atol(GridContext);
					break;
				case SA_dtDouble:
					g_dbcommand.Param(( SAString )FieldNameList.GetAt(mydlg.m_Index)).setAsDouble() = atof(GridContext);
					break;
				case SA_dtString:
					g_dbcommand.Param(( SAString )FieldNameList.GetAt(mydlg.m_Index)).setAsString() = GridContext;
					break;
				case SA_dtLongChar:
					g_dbcommand.Param(( SAString )FieldNameList.GetAt(mydlg.m_Index)).setAsLongChar() = strvalue;
					break;
				case SA_dtCLob:
					g_dbcommand.Param(( SAString )FieldNameList.GetAt(mydlg.m_Index)).setAsCLob() = strvalue;
					break;
				default:;
				}				
				
				g_dbcommand.Execute();
				g_dbconnection.Commit();
			}
			catch(SAException &)
			{
				try
				{
					g_dbconnection.Rollback();
				}
				catch(SAException &)
				{
				}
			}

		}

		FieldNameList.RemoveAll();
		FieldTypeList.RemoveAll();

		if(theApp.m_AutoRefreshDBListMode.CompareNoCase("是") == 0 && ((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.GetSafeHwnd())
		{
			((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.m_RecordList.ResetGridSize();
			((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.m_RecordList.RedrawAll();
		}
		//======================================================================
	}
}
