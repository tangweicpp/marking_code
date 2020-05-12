#include "frmworep.h"
#include "ui_frmworep.h"

FrmWORep::FrmWORep(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrmWORep)
{
    ui->setupUi(this);
    ui->splitter->setStretchFactor(1,1);
    ui->tableWidget->setShowGrid(true);
    ui->tableWidget->setGridStyle(Qt::DashDotLine);
    ui->tableWidget->setSortingEnabled(true);
    ui->tableWidget->setAlternatingRowColors(true);
    ui->tableWidget->setStyleSheet("QTableView { border: none;"
                                   "selection-background-color: #8EDE21;"
                                   "background-color:rgb(231, 231, 231);"
                                   "color: blue}");
}

FrmWORep::~FrmWORep()
{
    delete ui;
}

// Get file name
void FrmWORep::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,"打开WO文件","C:\\","excel Files(*.xlsx;*.xls;*.csv);;xlsx Files(*.xlsx);;xls Files(*.xls);;csv Files(*.csv)");
    if(fileName.isEmpty())
        return;

    ui->fileNameEdit->setText(fileName);
}

// Open file and get data
void FrmWORep::on_queryButton_clicked()
{
    ui->progressBar->setValue(0);
    if (ui->fileNameEdit->text().trimmed().isEmpty()) {
        QMessageBox::information(this,"提示","请选择你要上传的文件");
        return ;
    }

    if (ui->uploadTypeComboBox->currentText().trimmed().isEmpty()) {
        QMessageBox::information(this,"提示","请选择上传类型");
        return ;
    }
    qDebug() << ui->bondedTypeComboBox->currentText().trimmed();
    if (ui->bondedTypeComboBox->currentText().trimmed().isEmpty()) {
        QMessageBox::information(this,"提示","请选择保税类型");
        return ;
    }

    QVector<WODATA> v;
    if (getData(v) && saveData(v)) {
        QMessageBox::warning(this, "提示", "WO上传失败");
    } else {
        QMessageBox::information(this, "提示", "WO上传完成");

        // Clear
        ui->fileNameEdit->setText("");
        ui->bondedTypeComboBox->setCurrentIndex(0);
    }
}

// Get the data from the excel
bool FrmWORep::getData(QVector<WODATA> & v)
{
    WODATA data;

    QAxObject excel("Excel.Application");
    excel.setProperty("Visible", false);
    QAxObject *work_books = excel.querySubObject("WorkBooks");
    work_books->dynamicCall("Open (const QString&)", ui->fileNameEdit->text().trimmed());
    QAxObject *work_book = excel.querySubObject("ActiveWorkBook");
    QAxObject *work_sheets = work_book->querySubObject("Sheets");

    int sheet_count = work_sheets->property("Count").toInt();

    if (sheet_count > 0) {
        QAxObject *work_sheet = work_book->querySubObject("Sheets(int)", 1);
        QAxObject *used_range = work_sheet->querySubObject("UsedRange");
        QAxObject *row = used_range->querySubObject("Rows");
        QAxObject *col = used_range->querySubObject("Columns");

        int rows = row->property("Count").toInt();
        int cols = col->property("Count").toInt();

        // Get common value
        data.uploadID = CommApi::getOracleStr("select PO_ITEM_SEQ.nextval from dual");
        data.uploadName = ui->fileNameEdit->text().trimmed();
        data.uploadBy = CommApi::gUserName;
        data.uploadType = ui->uploadTypeComboBox->currentText().trimmed();
        data.bondedType = ui->bondedTypeComboBox->currentText().trimmed();

        for (int i = 2; i <= rows; i++) {
            // Set progressbar
            ui->progressBar->setValue(i * 80 / rows);

            // Get col value
            data.col_A = work_sheet->querySubObject("Cells(int,int)", i, 1)->property("Value").toString().trimmed();
            data.col_B = work_sheet->querySubObject("Cells(int,int)", i, 2)->property("Value").toString().trimmed();
            data.col_C = work_sheet->querySubObject("Cells(int,int)", i, 3)->property("Value").toString().trimmed();
            data.col_D = work_sheet->querySubObject("Cells(int,int)", i, 4)->property("Value").toString().trimmed();
            data.col_E = work_sheet->querySubObject("Cells(int,int)", i, 5)->property("Value").toString().trimmed();
            data.col_F = work_sheet->querySubObject("Cells(int,int)", i, 6)->property("Value").toString().trimmed();
            data.col_G = work_sheet->querySubObject("Cells(int,int)", i, 7)->property("Value").toString().trimmed();
            data.col_H = work_sheet->querySubObject("Cells(int,int)", i, 8)->property("Value").toString().trimmed();
            data.col_I = work_sheet->querySubObject("Cells(int,int)", i, 9)->property("Value").toString().trimmed();
            data.col_J = work_sheet->querySubObject("Cells(int,int)", i, 10)->property("Value").toString().trimmed();
            data.col_K = work_sheet->querySubObject("Cells(int,int)", i, 11)->property("Value").toString().trimmed();
            data.col_L = work_sheet->querySubObject("Cells(int,int)", i, 12)->property("Value").toString().trimmed();
            data.col_M = work_sheet->querySubObject("Cells(int,int)", i, 13)->property("Value").toString().trimmed();
            data.col_N = work_sheet->querySubObject("Cells(int,int)", i, 14)->property("Value").toString().trimmed();
            data.col_O = work_sheet->querySubObject("Cells(int,int)", i, 15)->property("Value").toString().trimmed();
            data.col_P = work_sheet->querySubObject("Cells(int,int)", i, 16)->property("Value").toString().trimmed();
            data.col_Q = work_sheet->querySubObject("Cells(int,int)", i, 17)->property("Value").toString().trimmed();
            data.col_R = work_sheet->querySubObject("Cells(int,int)", i, 18)->property("Value").toString().trimmed();
            data.col_S = work_sheet->querySubObject("Cells(int,int)", i, 19)->property("Value").toString().trimmed();
            data.col_T = work_sheet->querySubObject("Cells(int,int)", i, 20)->property("Value").toString().trimmed();
            data.col_U = work_sheet->querySubObject("Cells(int,int)", i, 21)->property("Value").toString().trimmed();

            // Get specified value
            data.custPO = work_sheet->querySubObject("Cells(int,int)", i, 2)->property("Value").toString().trimmed();
            data.custCode = work_sheet->querySubObject("Cells(int,int)", i, 3)->property("Value").toString().trimmed();
            data.custFabPN = work_sheet->querySubObject("Cells(int,int)", i, 5)->property("Value").toString().trimmed();
            data.custPN = work_sheet->querySubObject("Cells(int,int)", i, 6)->property("Value").toString().trimmed();
            data.custLotID = work_sheet->querySubObject("Cells(int,int)", i, 10)->property("Value").toString().trimmed();
            data.custWaferNo = work_sheet->querySubObject("Cells(int,int)", i, 11)->property("Value").toString().trimmed();
            data.goodDies = work_sheet->querySubObject("Cells(int,int)", i, 12)->property("Value").toString().toLong();
            data.grossDies = work_sheet->querySubObject("Cells(int,int)", i, 13)->property("Value").toString().toLong();
            data.htPN = work_sheet->querySubObject("Cells(int,int)", i, 14)->property("Value").toString().trimmed();
            data.custLaserMark = getLaserMark(data);

            if (data.custWaferNo.length() == 1) {
                data.custWaferID = data.custLotID + "0" + data.custWaferNo;
            } else {
                data.custWaferID = data.custLotID + data.custWaferNo;
            }

            // Check data




            if ( !checkData(data, i) )
                work_book->dynamicCall("Close(Boolean)", false);
                excel.dynamicCall("Quit(void)");
                return false;

            // Push back data
            v.push_back(data);
        }
    } else {
        work_book->dynamicCall("Close(Boolean)", false);
        excel.dynamicCall("Quit(void)");

        return false;
    }

    work_book->dynamicCall("Close(Boolean)", false);
    excel.dynamicCall("Quit(void)");
    return true;
}

// Check the Data
bool FrmWORep::checkData(WODATA & data, int col)
{
    QString sql;

    // 1. Check PN
    sql = QString("select * from tbltsvnpiproduct where customerptno1 = '%1' and qtechptno = '%2' and customershortname = '%3' ").arg(data.custPN).arg(data.htPN).arg(data.custCode);
    if (!CommApi::isDataExisted_oracle(sql)) {
        QMessageBox::warning(this, "警告", QString("NPI对照表查询不到第%1行的机种维护记录,请检查客户代码,客户机种,厂内机种").arg(col));
        return false;
    }

    return true;
}

// Save data to db
bool FrmWORep::saveData(QVector<WODATA> & v)
{
    ui->progressBar->setValue(100);
    return true;
}

// Get laserMark
QString FrmWORep::getLaserMark(WODATA & data)
{
    if (data.custCode == "SX" || data.custCode == "HJ" || data.custCode == "TJ003" || data.custCode == "JS140" || data.custCode == "BJ153") {
        if (data.htPN == "GY152AM" || data.htPN == "GY153AM" || data.htPN == "YSX002" || data.htPN == "XSX001" || data.htPN == "YSX004M" || data.htPN == "YSX005M" || data.htPN == "YSX006M") {
            return CommApi::getOracleStr(QString("SELECT QTMCodeSeq.SXCode8('%1','%2') FROM DUAL").arg(data.custLotID).arg(data.custWaferNo));
        } else {
            return CommApi::getOracleStr("SELECT QTMCodeSeq.SXCode('a') FROM DUAL");
        }
    }

    if (data.custPN == "GW1N-LV1CS30C6/I5") {
        return data.col_Q + "\\" + data.col_R + "\\" + data.col_S + "\\" + data.custLotID;
    }

    if (data.custPN == "GW1N-LV4CS72") {
        return data.col_Q + "\\" + data.col_R + "\\" + data.col_S + "\\" + data.custLotID;
    }


//    QDateTime::currentDateTime.toString("yyyy")

//                Case "GW1N-LV4CS72"
//                    If dT.DATA1 = "" Or dT.DATA2 = "" Or dT.DATA3 = "" Then
//                        MsgBox "Q列,R列,S列为打标码必需字段,不可为空", vbInformation, "警告"
//                        Exit Function

//                    End If

//                    dT.MARKING_CODE = dT.DATA1 & "\\" & dT.DATA2 & "\\" & Right(Year(Now), 2) & Right("0" & DatePart("WW", Now), 2) & "B" & "\\" & dT.Lot_id
//                    dT.MARKING_CODE2 = dT.DATA1 & "\\" & dT.DATA3 & "\\" & Right(Year(Now), 2) & Right("0" & DatePart("WW", Now), 2) & "B" & "\\" & dT.Lot_id

//            End Select

//        Case "69"
//            dT.MARKING_CODE = Mid(dT.Lot_id, 2, 6) & Mid("ABCDEFGHIJKLMNOPQRSTUVWXY", dT.wafer_id, 1)

//        Case "SG005", "TW079"
//            dT.MARKING_CODE = Mid$(dT.Customer_Device, InStr(dT.Customer_Device, "-") + 2, 1) & Right(Year(Now), 1) & Hex(Month(Now)) & Mid$("123456789ABCDEFGHIJKLMNOP", dT.wafer_id, 1)
//            If InStr(dT.Lot_id, ".") > 0 Then
//                dT.MARKING_CODE = dT.MARKING_CODE & Mid$(dT.Lot_id, InStr(dT.Lot_id, ".") - 4, 4)
//            Else
//                dT.MARKING_CODE = dT.MARKING_CODE & Right$(dT.Lot_id, 4)

//            End If

//        Case "US026"
//            If dT.Customer_Device = "TM2G1" Then
//                dT.MARKING_CODE = " " & Right(Year(Now), 1) & Mid("123456789ABC", Month(Now), 1) & Mid$("123456789ABCDEFGHIJKLMNOP", dT.wafer_id, 1) & Right(Left(dT.Lot_id, InStr(dT.Lot_id, ".") - 1), 4)
//            Else
//                dT.MARKING_CODE = Mid$(dT.Customer_Device, InStr(dT.Customer_Device, "-") + 2, 1) & Right(Year(Now), 1) & Hex(Month(Now)) & Mid$("123456789ABCDEFGHIJKLMNOP", dT.wafer_id, 1)
//                If InStr(dT.Lot_id, ".") > 0 Then
//                    dT.MARKING_CODE = dT.MARKING_CODE & Mid$(dT.Lot_id, InStr(dT.Lot_id, ".") - 4, 4)
//                Else
//                    dT.MARKING_CODE = dT.MARKING_CODE & Right$(dT.Lot_id, 4)

//                End If

//            End If

//        Case "TW067"
//            If Len(dT.DATA1) <> 5 Then
//                MsgBox "Q列必须有5位信息供打标码取用", vbInformation, "提示"
//                Exit Function

//            End If

//            dT.MARKING_CODE = dT.DATA1 & Mid$("123456789ABCDEFGHJKLMNPQRSTUVW", dT.wafer_id, 1)

//            '        Select Case dT.Customer_Device 杨朝栋负责
//            '
//            '            Case "PS5250LT", "PS5250LT-AA", "PS5260LT", "PS5250LT-AA"
//            '                dT.MARKING_CODE = dT.DATA1 & Mid$("123456789ABCDEFGHJKLMNPQRSTUVW", dT.Wafer_id, 1)
//            '
//            '             Case Else
//            '
//            '
//            '
//            '
//            '        End Select
//        Case "SH192"
//            If dT.HT_DEVICE = "XSH192002" Then
//                If InStr(dT.Lot_id, ".") > 0 Then
//                    dT.MARKING_CODE = "HTG6C" + "\\" + Mid(dT.Lot_id, InStr(dT.Lot_id, ".") - 4, 4) + "\\" + Trim(Right(Year(Now), 2)) + Right("0" & DatePart("WW", Now), 2)
//                Else
//                    dT.MARKING_CODE = "HTG6C" + "\\" + Right(dT.Lot_id, 4) + "\\" + Trim(Right(Year(Now), 2)) + Right("0" & DatePart("WW", Now), 2)

//                End If

//            End If

//        Case "SH115"
//            dT.MARKING_CODE = Mid(dT.Customer_Device, 3, 4) + "\\" + Trim(Right(Year(Now), 2)) + Right("0" & DatePart("WW", Now), 2)

//        Case "KR001"

//            Select Case dT.Customer_Device

//                Case "PC7090K"
//                    dT.MARKING_CODE = GetKRMark(dT.Lot_id, dT.wafer_id)

//                    ' Changed by: Project Administrator at: 2019/9/9-13:28:52 on machine: DESKTOP-MSUG5JD 杨朝栋 要求除了PC7090K,其他都走统一格式
//                    '            Case "PS1130K", "PS4210K", "PC7080D", "PK2130K", "PCB030K", "PK3130K", "PV3109K"
//                    '                dT.MARKING_CODE = GetKRMarkP(dT.Lot_id, dT.Wafer_id)
//                Case Else
//                    dT.MARKING_CODE = GetKRMarkP(dT.Lot_id, dT.wafer_id)

//            End Select

//        Case "KR002"
//            dT.MARKING_CODE = Right$(dT.Lot_id, 2) & Right$("0" & dT.wafer_id, 2)

//        Case "KR009"
//            If UCase(dT.Customer_Device) = "HI-1A1" Then
//                dT.MARKING_CODE = Right("0" & dT.wafer_id, 2) & "2" & Mid(dT.Lot_id, 5, 3)

//            End If

//        Case "HY"
//            If UCase(dT.Customer_Device) = "HI-258" Then
//                dT.MARKING_CODE = Right("0" & dT.wafer_id, 2) & "2" & Mid(dT.Lot_id, 5, 3)

//            End If

//        Case "AT71", "AH033", "SZ280"

//            Select Case dT.Customer_Device

//                Case "FP5513E4"
//                    If dT.DATA2 = "" Or dT.DATA3 = "" Then
//                        MsgBox "市场部必须填写R列和S列的值,以供打标码拼接", vbExclamation, "提示"
//                        Exit Function

//                    End If

//                    dT.MARKING_CODE = dT.DATA2 & dT.DATA3

//                Case "FP5510EE4"
//                    If dT.DATA2 = "" Or dT.DATA3 = "" Then
//                        MsgBox "市场部必须填写R列和S列的值,以供打标码拼接", vbExclamation, "提示"
//                        Exit Function

//                    End If

//                    dT.MARKING_CODE = "8a" & Right$(Year(Now), 1) & "\\" & Hex(Month(Now)) & Right$(dT.DATA3, 2)

//                Case "FP5510FE4"
//                    If dT.DATA2 = "" Or dT.DATA3 = "" Then
//                        MsgBox "市场部必须填写R列和S列的值,以供打标码拼接", vbExclamation, "提示"
//                        Exit Function

//                    End If

//                    dT.MARKING_CODE = "9-" & Right$(Year(Now), 1) & "\\" & Hex(Month(Now)) & Right$(dT.DATA3, 2)

//                Case "FP5519E4"
//                    If dT.DATA2 = "" Or dT.DATA3 = "" Then
//                        MsgBox "市场部必须填写R列和S列的值,以供打标码拼接", vbExclamation, "提示"
//                        Exit Function

//                    End If

//                    dT.MARKING_CODE = "7-" & Right$(Year(Now), 1) & "\\" & Hex(Month(Now)) & Right$(dT.DATA3, 2)

//                Case "FP5510E2"
//                    If dT.DATA2 = "" Or dT.DATA3 = "" Then
//                        MsgBox "市场部必须填写R列和S列的值,以供打标码拼接", vbExclamation, "提示"
//                        Exit Function

//                    End If

//                    dT.MARKING_CODE = "2=" & Right$(Year(Now), 1) & "\\" & Hex(Month(Now)) & Right$(dT.DATA3, 2)

//                Case "FP5510EE4AEE"
//                    If dT.DATA2 = "" Or dT.DATA3 = "" Then
//                        MsgBox "市场部必须填写R列和S列的值,以供打标码拼接", vbExclamation, "提示"
//                        Exit Function

//                    End If

//                    dT.MARKING_CODE = "8a" & Right$(Year(Now), 1) & "\\" & Hex(Month(Now)) & Right$(dT.DATA3, 2)

//                Case "FP5516WE4"
//                    If dT.DATA2 = "" Or dT.DATA3 = "" Then
//                        MsgBox "市场部必须填写R列和S列的值,以供打标码拼接", vbExclamation, "提示"
//                        Exit Function

//                    End If

//                    dT.MARKING_CODE = "5+" & Right$(Year(Now), 1) & "\\" & Hex(Month(Now)) & Right$(dT.DATA3, 2)

//            End Select

//            Select Case dT.HT_DEVICE

//                Case "XAT71023B"
//                    If dT.DATA2 = "" Or dT.DATA3 = "" Then
//                        MsgBox "市场部必须填写R列和S列的值,以供打标码拼接", vbExclamation, "提示"
//                        Exit Function

//                    End If

//                    dT.MARKING_CODE = "1+" & Right$(Year(Now), 1) & "\\" & Hex(Month(Now)) & Right$(dT.DATA3, 2)

//                Case "XAT71019B"
//                    If dT.DATA2 = "" Or dT.DATA3 = "" Then
//                        MsgBox "市场部必须填写R列和S列的值,以供打标码拼接", vbExclamation, "提示"
//                        Exit Function

//                    End If

//                    dT.MARKING_CODE = "5-" & Right$(Year(Now), 1) & "\\" & Hex(Month(Now)) & Right$(dT.DATA3, 2)

//                Case "XAT71024B"
//                    If dT.DATA2 = "" Or dT.DATA3 = "" Then
//                        MsgBox "市场部必须填写R列和S列的值,以供打标码拼接", vbExclamation, "提示"
//                        Exit Function

//                    End If

//                    dT.MARKING_CODE = "3=" & Right$(Year(Now), 1) & "\\" & Hex(Month(Now)) & Right$(dT.DATA3, 2)

//            End Select

//        Case "RD"
//            If dT.Customer_Device = "RDA2216" Then
//                dT.MARKING_CODE = "RDA" & "2216" & Mid(dT.Lot_id, 3, 4) & Right$("0" & dT.wafer_id, 2)

//            End If

//        Case "AB18"
//            dT.MARKING_CODE = Replace(dT.MARKING_CODE, "****", Trim(Right(Year(Now), 2)) + Right("0" & DatePart("WW", Now), 2))

//        Case "SD"
//            If dT.Customer_Device = "SD12" Then
//                dT.MARKING_CODE = "SD12" & "\\" & Mid(dT.Lot_id, 2, 6)

//            End If

//        Case "SH103"
//            'dT.MARKING_CODE = Right$(dT.Customer_Device, 4) & "\\" & Left$(dT.WAFER_VERSION, 4) & "\\" & Right$(dT.WAFER_VERSION, 2)

//            Select Case dT.HT_DEVICE

//                Case "XSH103003"    ' 闫雪婷, ' Changed by: Project Administrator at: 2019/8/14-14:28:23 on machine: DESKTOP-MSUG5JD
//                    If Len(dT.WAFER_VERSION) <> 4 Then
//                        MsgBox "市场部必须填写G列的值,且G列必须是4位,以供打标码拼接", vbExclamation, "提示"
//                        Exit Function

//                    End If

//                    dT.MARKING_CODE = "K" & Left(dT.WAFER_VERSION, 2) & "\\" & Right(dT.WAFER_VERSION, 2)

//            End Select

//        Case "DA69"

//            Select Case dT.HT_DEVICE

//                Case "XDA69001B"
//                    dT.MARKING_CODE = "46A" & "\\" & Right$(Year(Now), 1) & Right("0" & DatePart("WW", Now), 2)

//                Case "XDA69002B"
//                    dT.MARKING_CODE = "772" & "\\" & Right$(Year(Now), 1) & Right("0" & DatePart("WW", Now), 2)

//                Case "XDA69003B"
//                    dT.MARKING_CODE = "96B" & "\\" & Right$(Year(Now), 1) & Right("0" & DatePart("WW", Now), 2)

//                Case "XDA69004B"
//                    dT.MARKING_CODE = "64BA" & "\\" & Right$(Year(Now), 1) & Right("0" & DatePart("WW", Now), 2)

//                Case "XDA69006B"
//                    ' Changed by: Project Administrator at: 2019/8/19-10:23:30 on machine: DESKTOP-MSUG5JD 王雪
//                    dT.MARKING_CODE = "97C" & "\\" & Right$(Year(Now), 1) & Right("0" & DatePart("WW", Now), 2)

//                Case "XDA69A03B"
//                    dT.MARKING_CODE = "96U" & "\\" & Right$(Year(Now), 1) & Right("0" & DatePart("WW", Now), 2)

//                Case "XDA69005B"    ' 2019下半年C,2020上半年D,2020下半年E,依次递增
//                    'dT.MARKING_CODE = "769" & "\\" & "W" & "C" & Mid("ABCDEFGHIJKLMNOPQRSTUVWXYZ3BCDEFGHIJKLMNOPQRSTUVWXY456", DatePart("WW", Now), 1)
//                     dT.MARKING_CODE = "769" & "\\" & "W" & "D" & Mid("ABCDEFGHIJKLMNOPQRSTUVWXYZ3BCDEFGHIJKLMNOPQRSTUVWXY456", DatePart("WW", Now), 1)


//                Case "XDA69007B"
//                    dT.MARKING_CODE = "768" & "\\" & Right$(Year(Now), 1) & Right("0" & DatePart("WW", Now), 2)

//                Case "XDA69B03B"
//                    dT.MARKING_CODE = "96W" & "\\" & Right$(Year(Now), 1) & Right("0" & DatePart("WW", Now), 2)

//                Case "XDA69A06B"
//                    dT.MARKING_CODE = "97U" & "\\" & Right$(Year(Now), 1) & Right("0" & DatePart("WW", Now), 2)

//            End Select

//        Case "AC64"

//            Select Case dT.HT_DEVICE

//                Case "XAC64005B", "XAC64002B", "XAC64009B", "XAC64008B", "XAC64014B", "XAC64A08B", "XAC64B08B", "XAC64C08B"
//                    If dT.WAFER_VERSION = "" Then
//                        MsgBox "市场部必须填写G列的值,以供打标码拼接", vbExclamation, "提示"
//                        Exit Function

//                    End If

//                    dT.MARKING_CODE = "LUB" & "\\" & dT.WAFER_VERSION

//                Case "XAC64011B"
//                    If dT.WAFER_VERSION = "" Then
//                        MsgBox "市场部必须填写G列的值,以供打标码拼接", vbExclamation, "提示"
//                        Exit Function

//                    End If

//                    dT.MARKING_CODE = "FLB" & "\\" & dT.WAFER_VERSION

//                Case "XAC64013B"
//                    If dT.WAFER_VERSION = "" Then
//                        MsgBox "市场部必须填写G列的值,以供打标码拼接", vbExclamation, "提示"
//                        Exit Function

//                    End If

//                    dT.MARKING_CODE = "LVB" & "\\" & dT.WAFER_VERSION

//                Case "XAC64006B"
//                    If dT.WAFER_VERSION = "" Then
//                        MsgBox "市场部必须填写G列的值,以供打标码拼接", vbExclamation, "提示"
//                        Exit Function

//                    End If

//                    dT.MARKING_CODE = "92011" & "\\" & dT.WAFER_VERSION

//                Case "XAC64007B"
//                    If dT.WAFER_VERSION = "" Then
//                        MsgBox "市场部必须填写G列的值,以供打标码拼接", vbExclamation, "提示"
//                        Exit Function

//                    End If

//                    dT.MARKING_CODE = "92012" & "\\" & dT.WAFER_VERSION

//                Case "XAC64012B"
//                    If dT.WAFER_VERSION = "" Then
//                        MsgBox "市场部必须填写G列的值,以供打标码拼接", vbExclamation, "提示"
//                        Exit Function

//                    End If

//                    dT.MARKING_CODE = "LYB" & "\\" & dT.WAFER_VERSION

//                Case "XAC64A01B"
//                    If dT.WAFER_VERSION = "" Then
//                        MsgBox "市场部必须填写G列的值,以供打标码拼接", vbExclamation, "提示"
//                        Exit Function

//                    End If

//                    dT.MARKING_CODE = "KLB" & "\\" & dT.WAFER_VERSION

//                Case "XAC64A12B"
//                    If dT.WAFER_VERSION = "" Then
//                        MsgBox "市场部必须填写G列的值,以供打标码拼接", vbExclamation, "提示"
//                        Exit Function

//                    End If

//                    '  dT.MARKING_CODE = "LYB" & "\\" & dT.WAFER_VERSION
//                    ' dT.MARKING_CODE = "KLB" & "\\" & dT.WAFER_VERSION
//                    dT.MARKING_CODE = "LYB" & "\\" & dT.WAFER_VERSION

//                Case "XAC64B01B"
//                    If dT.WAFER_VERSION = "" Then
//                        MsgBox "市场部必须填写G列的值,以供打标码拼接", vbExclamation, "提示"
//                        Exit Function

//                    End If

//                    dT.MARKING_CODE = "KLB" & "\\" & dT.WAFER_VERSION

//                Case "XAC64C01B"
//                    If dT.WAFER_VERSION = "" Then
//                        MsgBox "市场部必须填写G列的值,以供打标码拼接", vbExclamation, "提示"
//                        Exit Function

//                    End If

//                    dT.MARKING_CODE = "QLC" & "\\" & dT.WAFER_VERSION

//                Case "XAC64A13B"
//                    If dT.WAFER_VERSION = "" Then
//                        MsgBox "市场部必须填写G列的值,以供打标码拼接", vbExclamation, "提示"
//                        Exit Function

//                    End If

//                    dT.MARKING_CODE = "LVB" & "\\" & dT.WAFER_VERSION

//            End Select

//        Case "QR"

//            Select Case dT.Customer_Device

//                Case "MT01", "AX01"
//                    dT.MARKING_CODE = Right(dT.Lot_id, 4) & Mid$("ABCDEFGHIJKLMNOPQRSTUVWXY", dT.wafer_id, 1)

//            End Select

//        Case "SH267"    ' 冯静20190506
//            '        Select Case dT.Customer_Device
//            '
//            '            Case "SC2238", "VENUS", "EIAR"
//            '                dT.MARKING_CODE = Right(Year(Now), 1) & UCase(Hex(Month(Now))) & Mid$(dT.lot_wafer_id, 3, 4) & Mid$("123456789ABCDEFGHJKLMNPQR", dT.Wafer_id, 1)
//            '
//            '        End Select
//            ' 冯静20190530 所有客户机种走一套
//            dT.MARKING_CODE = Right(Year(Now), 1) & UCase(Hex(Month(Now))) & Mid$(dT.lot_wafer_id, 3, 4) & Mid$("123456789ABCDEFGHJKLMNPQR", dT.wafer_id, 1)

//        Case "HD"

//            Select Case dT.Customer_Device

//                Case "GH610", "GH611", "GH612"   ' 黄鹤鸣 20190523
//                    dT.MARKING_CODE = dT.Customer_Device & "\\" & Split(dT.DATA5, "-")(0) & "\\" & Split(dT.DATA5, "-")(1)

//            End Select

//            Select Case dT.HT_DEVICE

//                Case "XHD004B"
//                    dT.MARKING_CODE = dT.Customer_Device & "\\" & Split(dT.DATA5, "-")(0) & "-" & Split(dT.DATA5, "-")(1) & "\\" & Right$("00" & dT.wafer_id, 2)

//            End Select

//            '    Case "AH017"
//            '        If Len(dT.Customer_Device) = 11 Then
//            '            dT.MARKING_CODE = Mid(dT.Customer_Device, 3, 5) & "\\" & Right(Year(Now), 2) & Right("0" & DatePart("WW", Now), 2) & "\\" & Mid$("ABCDEFGHIJKLMNOPQRSTUVWXY", dT.Wafer_id, 1) & Mid$(dT.Lot_id, InStr(dT.Lot_id, ".") - 3, 3) & "\\" & Mid(dT.Customer_Device, 7, 3)
//            '        ElseIf Len(dT.Customer_Device) = 10 Then
//            '            dT.MARKING_CODE = Mid(dT.Customer_Device, 3, 4) & "\\" & Right(Year(Now), 2) & Right("0" & DatePart("WW", Now), 2) & "\\" & Mid$("ABCDEFGHIJKLMNOPQRSTUVWXY", dT.Wafer_id, 1) & Mid$(dT.Lot_id, InStr(dT.Lot_id, ".") - 3, 3) & "\\" & Mid(dT.Customer_Device, 7, 3)
//            '        Else
//            '            MsgBox "客户机种位数不正确", vbCritical, "警告"
//            '            Exit Function
//            '
//            '        End If
//        Case "SZ217"    ' 王雪 20190611
//            dT.MARKING_CODE = "ST2018"

//        Case "AC70"

//            Select Case dT.HT_DEVICE

//                Case "XAC7013B"
//                    If Len(Trim(dT.REMARK)) <> 4 Then
//                        MsgBox "O列中必须包含4位打标码信息", vbInformation, "警告"
//                        Exit Function

//                    End If

//                    dT.MARKING_CODE = "7F7L" & "\\" & dT.REMARK

//                Case "XAC7018B"
//                    If Len(Trim(dT.REMARK)) <> 4 Then
//                        MsgBox "O列中必须包含4位打标码信息", vbInformation, "警告"
//                        Exit Function

//                    End If

//                    dT.MARKING_CODE = "K318" & "\\" & dT.REMARK

//                Case "XAC7016B"
//                    If Len(Trim(dT.REMARK)) <> 4 Then
//                        MsgBox "O列中必须包含4位打标码信息", vbInformation, "警告"
//                        Exit Function

//                    End If

//                    dT.MARKING_CODE = "AWINIC" & "\\" & "87339" & "\\" & dT.REMARK

//                Case "XAC7009B"
//                    If Len(Trim(dT.REMARK)) <> 4 Then
//                        MsgBox "O列中必须包含4位打标码信息", vbInformation, "警告"
//                        Exit Function

//                    End If

//                    dT.MARKING_CODE = "K327" & "\\" & dT.REMARK

//                    '            Case "XAC70A2B"
//                    '                If Len(Trim(dT.REMARK)) <> 4 Then
//                    '                    MsgBox "O列中必须包含4位打标码信息", vbInformation, "警告"
//                    '                    Exit Function
//                    '
//                    '                End If
//                    '
//                    '                dT.MARKING_CODE = "3805" & "\\" & dT.REMARK
//                Case "XAC7015B"
//                    If Len(Trim(dT.REMARK)) <> 4 Then
//                        MsgBox "O列中必须包含4位打标码信息", vbInformation, "警告"
//                        Exit Function

//                    End If

//                    dT.MARKING_CODE = "C031" & "\\" & dT.REMARK

//                Case "XAC7006B"
//                    If Len(Trim(dT.REMARK)) <> 4 Then
//                        MsgBox "O列中必须包含4位打标码信息", vbInformation, "警告"
//                        Exit Function

//                    End If

//                    dT.MARKING_CODE = "K37S" & "\\" & dT.REMARK

//                Case "XAC7017B"
//                    If Len(Trim(dT.REMARK)) <> 4 Then
//                        MsgBox "O列中必须包含4位打标码信息", vbInformation, "警告"
//                        Exit Function

//                    End If

//                    dT.MARKING_CODE = "DGY3" & "\\" & dT.REMARK

//                Case "XAC7024B"
//                    If Len(Trim(dT.REMARK)) <> 4 Then
//                        MsgBox "O列中必须包含4位打标码信息", vbInformation, "警告"
//                        Exit Function

//                    End If

//                    dT.MARKING_CODE = "UV25" & "\\" & dT.REMARK

//                Case "XAC7022B"
//                    If Len(Trim(dT.REMARK)) <> 4 Then
//                        MsgBox "O列中必须包含4位打标码信息", vbInformation, "警告"
//                        Exit Function

//                    End If

//                    dT.MARKING_CODE = "X4LV" & "\\" & dT.REMARK

//                Case "XAC7019B"
//                    If Len(Trim(dT.REMARK)) <> 4 Then
//                        MsgBox "O列中必须包含4位打标码信息", vbInformation, "警告"
//                        Exit Function

//                    End If

//                    dT.MARKING_CODE = "MYCOCY" & "\\" & dT.REMARK

//            End Select

//        Case "ZJ116"

//            Select Case dT.HT_DEVICE

//                Case "XZJ11601B"
//                    If Len(Trim(dT.MARKING_CODE)) <> 10 Then
//                        MsgBox "H列不正确", vbInformation, "警告"
//                        Exit Function

//                    End If

//                    If InStr(dT.MARKING_CODE, "\\") = 0 Then
//                        MsgBox "H列格式不正确", vbInformation, "警告"
//                        Exit Function

//                    End If

//            End Select

//        Case "HW106", "HK093"

//            Select Case dT.HT_DEVICE

//                Case "XHW10601M", "XHW10602M", "XHW10603M"   ' Changed by: Project Administrator at: 2019/8/14-14:28:58 on machine: DESKTOP-MSUG5JD 谭双强
//                    dT.MARKING_CODE = "A" & Right(Year(Now), 1) & Hex(Month(Now)) & Mid$("123456789ABCDEFGHIJKLMNOP", dT.wafer_id, 1) & Mid$(dT.Lot_id, 3, 5)

//            End Select

//        Case "SH105"

//            Select Case dT.HT_DEVICE

//                Case "XSH10501B"
//                    If dT.DATA1 = "" Then
//                        MsgBox "Q列不可为空", vbInformation, "警告"
//                        Exit Function

//                    End If

//                    dT.MARKING_CODE = Mid(dT.DATA1, 5, 6) & "\\" & Mid(dT.Lot_id, 2, 6)

//            End Select

//        Case "AC51"

//            Select Case dT.HT_DEVICE

//                Case "XAC51008B", "XAC51007B"
//                    If Len(dT.MARKING_CODE) <> 3 Then
//                        MsgBox "H列必须提供3位打标码", vbInformation, "警告"
//                        Exit Function

//                    End If

//                    dT.MARKING_CODE = "1646" & "\\" & dT.MARKING_CODE

//            End Select

//    End Select

//    Select Case dT.HT_DEVICE

//        Case "XFJ05701B"
//            dT.MARKING_CODE = Right$(dT.Customer_Device, 5) & "\\" & Right(Year(Now), 2) & Right("0" & DatePart("WW", Now), 2)    ' 20190926 闫雪婷 OA

//        Case "XSH103A01", "XSH103001"
//            If Len(dT.WAFER_VERSION) <> 6 Then
//                MsgBox "市场部必须填写G列的值,且G列必须是6位,以供打标码拼接", vbExclamation, "提示"
//                Exit Function

//            End If

//            dT.MARKING_CODE = "7983" & "\\" & Left(dT.WAFER_VERSION, 4) & "\\" & Right(dT.WAFER_VERSION, 2) ' 20190925 闫雪婷 OA

//        Case "XAC7023B"
//            If Len(dT.REMARK) <> 2 Then
//                MsgBox "市场部必须填写O列的值,且O列必须是2位,以供打标码拼接", vbExclamation, "提示"
//                Exit Function

//            End If

//          '  dT.MARKING_CODE = dT.REMARK & "\\" & "Z8" ' 20190930 崔家兴 MAIL
//            dT.MARKING_CODE = "8Z" & "\\" & dT.REMARK '20191212 张凯更新规则


//        Case "X76006B"
//            If CLng(dT.wafer_id) < 13 Or CLng(dT.wafer_id) > 19 Then
//                MsgBox "waferID不可小于13或大于19,请联系IT", vbCritical, "错误"
//                Exit Function

//            End If

//            If CLng(dT.wafer_id) >= 13 And CLng(dT.wafer_id) <= 15 Then
//                dT.MARKING_CODE = "DC-1" & "\\" & Mid$("KABCDE", Year(Now) - 2018, 1) & Mid("0123456789ACDEFHKLNPRSTUXYZ", (DatePart("WW", Now) + 1) \ 2, 1) & "P2"
//            ElseIf CLng(dT.wafer_id) >= 16 And CLng(dT.wafer_id) <= 17 Then
//                dT.MARKING_CODE = "DC-2" & "\\" & Mid$("KABCDE", Year(Now) - 2018, 1) & Mid("0123456789ACDEFHKLNPRSTUXYZ", (DatePart("WW", Now) + 1) \ 2, 1) & "P2"
//            Else
//                dT.MARKING_CODE = "DC-3" & "\\" & Mid$("KABCDE", Year(Now) - 2018, 1) & Mid("0123456789ACDEFHKLNPRSTUXYZ", (DatePart("WW", Now) + 1) \ 2, 1) & "P2"

//            End If

//        Case "X76008B"
//            dT.MARKING_CODE = "6D" & Mid(dT.Lot_id, 5, 2) & "\\" & Mid$("KABCDE", Year(Now) - 2018, 1) & Mid("0123456789ACDEFHKLNPRSTUXYZ", (DatePart("WW", Now) + 1) \ 2, 1) & "P2"

//        Case "X76007B"
//            dT.MARKING_CODE = "VJ" & Mid(dT.Lot_id, 9, 2) & "\\" & Mid$("KABCDE", Year(Now) - 2018, 1) & Mid("0123456789ACDEFHKLNPRSTUXYZ", (DatePart("WW", Now) + 1) \ 2, 1) & "P2"

//        Case "X76010B"
//            dT.MARKING_CODE = "6F" & Mid(dT.Lot_id, 5, 2) & "\\" & Mid$("KABCDE", Year(Now) - 2018, 1) & Mid("0123456789ACDEFHKLNPRSTUXYZ", (DatePart("WW", Now) + 1) \ 2, 1) & "P2"

//        Case "XSH21801B"
//            dT.MARKING_CODE = "SCE"

//        Case "Y68559B"
//            dT.MARKING_CODE = "BNA" & "\\" & Mid$("KMNPRSTVWXYZ", Year(Now) - 2018, 1) & Right("00" & DatePart("WW", Now), 2) & "\\" & Right(dT.Fab_Device, 3)



    if (data.htPN == "XSH48002B") {
        return "7" +  CommApi::currYear().right(1) + QString("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz").mid(CommApi::currWeek().toInt()-1, 1);
    }

    return data.col_H;
}

//bool FrmWORep::checkData(WODATA & data)
//{
//    QString sql;
//    int i = 0;
//    for (QVector<WODATA>::iterator it = v.begin(); it != v.end(); it++, i++) {

//        // 1 Check PN
//        //        sql = QString("select * from tbltsvnpiproduct where customerptno1 = '%1' and qtechptno = '%2'").arg(it->custPN).arg(it->htPN);
//        //        qDebug() << i << sql;
//        //        if (!CommApi::isDataExisted_oracle(sql)) {
//        //            QMessageBox::warning(this, "警告", QString("NPI对照表查询不到第%1行的机种维护记录").arg(i+1));

//        //            return false;
//        //        }

//        // 2 Show col data
//        // qDebug() << it->col_A << it->col_B << it->col_C << it->col_D << it->col_E << it->col_F << it->col_G << it->col_H << it->col_I << it->col_J << it->col_K << it->col_L << it->col_M << it->col_N << it->col_O << it->col_P << it->col_Q << it->col_R << it->col_S << it->col_T << it->col_U;
//        qDebug() << it->uploadID;
//    }

//    // Set progress bar
//    ui->progressBar->setValue(90);
//    return true;
//}

void FrmWORep::on_newButton_2_clicked()
{
    qDebug() << CommApi::currYear() << CommApi::currMonth() << CommApi::currDay() << CommApi::currWeek();
    qDebug() << "7" + CommApi::currYear().right(1) + QString("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz").mid(CommApi::currWeek().toInt()-1, 1);
}
