#include "robotcontrol.h"
#include "ui_robotcontrol.h"
#include <QPixmap>
#include <array>

RobotControl::RobotControl(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::RobotControl)
{
    ui->setupUi(this);

    QVector<QLineEdit*> edpos_ = {ui->eDPos_1, ui->eDPos_2, ui->eDPos_3, ui->eDPos_4, ui->eDPos_5};
    QVector<QLineEdit*> edneg_ = {ui->eDNeg_1, ui->eDNeg_2, ui->eDNeg_3, ui->eDNeg_4, ui->eDNeg_5};
    QVector<QLineEdit*> ddpos_ = {ui->dDPos_1, ui->dDPos_2, ui->dDPos_3, ui->dDPos_4, ui->dDPos_5};
    QVector<QLineEdit*> ddneg_ = {ui->dDNeg_1, ui->dDNeg_2, ui->dDNeg_3, ui->dDNeg_4, ui->dDNeg_5};
    QVector<QLineEdit*> etpos_ = {ui->eTPos_1, ui->eTPos_2, ui->eTPos_3, ui->eTPos_4, ui->eTPos_5};
    QVector<QLineEdit*> etneg_ = {ui->eTNeg_1, ui->eTNeg_2, ui->eTNeg_3, ui->eTNeg_4, ui->eTNeg_5};
    QVector<QLineEdit*> vrpos_ = {ui->vRPos_1, ui->vRPos_2, ui->vRPos_3, ui->vRPos_4, ui->vRPos_5};
    QVector<QLineEdit*> vrneg_ = {ui->vRNeg_1, ui->vRNeg_2, ui->vRNeg_3, ui->vRNeg_4, ui->vRNeg_5};
    QVector<QLineEdit*> vrposlow_ = {ui->vRPosLow_1, ui->vRPosLow_2, ui->vRPosLow_3, ui->vRPosLow_4, ui->vRPosLow_5};
    QVector<QLineEdit*> vrneglow_ = {ui->vRNegLow_1, ui->vRNegLow_2, ui->vRNegLow_3, ui->vRNegLow_4, ui->vRNegLow_5};
    QVector<QLineEdit*> vwpos_ = {ui->vWPos_1, ui->vWPos_2, ui->vWPos_3, ui->vWPos_4, ui->vWPos_5};
    QVector<QLineEdit*> vwneg_ =  {ui->vWNeg_1, ui->vWNeg_2, ui->vWNeg_3, ui->vWNeg_4, ui->vWNeg_5};

    setLine(&edpos, &edpos_);
    setLine(&edneg, &edneg_);
    setLine(&ddpos, &ddpos_);
    setLine(&ddneg, &ddneg_);
    setLine(&etpos, &etpos_);
    setLine(&etneg, &etneg_);
    setLine(&vrpos, &vrpos_);
    setLine(&vrneg, &vrneg_);
    setLine(&vrposlow, &vrposlow_);
    setLine(&vrneglow, &vrneglow_);
    setLine(&vwpos, &vwpos_);
    setLine(&vwneg, &vwneg_);

    getDefault();

    QPixmap pixleft("/home/fatih/Learn Qt/0.Project/GUIFukuroRobotControl/resource/guicomp2.png");
    QPixmap pixright("/home/fatih/Learn Qt/0.Project/GUIFukuroRobotControl/resource/guicomp1.png");
    QPixmap fukuro("/home/fatih/Learn Qt/0.Project/GUIFukuroRobotControl/resource/logoFukuro.png");
    ui->logoAttribute->setPixmap(fukuro.scaled(90, 90, Qt::KeepAspectRatio));
    ui->leftAttribute->setPixmap(pixleft.scaled(50, 600, Qt::KeepAspectRatio));
    ui->rightAttribute->setPixmap(pixright.scaled(50, 600, Qt::KeepAspectRatio));

    ui->customPlot->setBackground(QColor(60,60,60));
    ui->customPlot->xAxis->setLabel("Time");
    ui->customPlot->xAxis->setBasePen(QPen(QColor(250,250,250)));
    ui->customPlot->xAxis->setLabelColor(QColor(250,250,250));
    ui->customPlot->xAxis->setTickLabelColor(QColor(250,250,250));
    ui->customPlot->yAxis->setLabel("Velocity");
    ui->customPlot->yAxis->setBasePen(QPen(QColor(250,250,250)));
    ui->customPlot->yAxis->setLabelColor(QColor(250,250,250));
    ui->customPlot->yAxis->setTickLabelColor(QColor(250,250,250));

    ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
}

RobotControl::~RobotControl()
{
    delete ui;
}

void RobotControl::setParam(Variable *x, std::string ids, QComboBox *cb, QVector<QLineEdit*> line, int length)
{
    x->id = ids;
    x->shape = cb->currentIndex();
    for(int i = 0; i < length; i++){
        x->params.append(line[i]->text().toDouble());
    }
}

void RobotControl::setParam(Variable *x, std::string ids, int shape_, QVector<float> par, int length)
{
    x->id = ids;
    x->shape = shape_;
    qDebug() << x->shape;
    for(int i = 0; i < length ; i++){
        x->params.append(par[i]);
    }
}

void RobotControl::setLine(QVector<QLineEdit*>* pos, QVector<QLineEdit*>* pos_)
{
    qDebug() << pos_->length();
    for(int i=0; i<5; i++){
        pos->append(pos_->at(i));
    }
}

void RobotControl::getDefault()
{
    QString filename = "/home/fatih/fukuro2.json";

    QFile file(filename);

    if(!file.open(QIODevice::ReadOnly)){
        qDebug() << "Failed to open " << filename;
        return;
    }else{
        qDebug() << "succeed! " << filename;
    }

    QByteArray bytes = file.readAll();
    file.close();

    QJsonParseError jsonError;
    QJsonDocument docs = QJsonDocument::fromJson(bytes, &jsonError);

    if(jsonError.error != QJsonParseError::NoError){
        std::cout << "fromJson failed: " << jsonError.errorString().toStdString() << std::endl;
        return;
    }

    QJsonObject jsonObj = docs.object();

    getSetParams(&eDPos, jsonObj, "eD", "Positive", &edpos, ui->edPosComboBox);
    getSetParams(&eDNeg, jsonObj, "eD", "Negative", &edneg, ui->edNegComboBox);
    getSetParams(&dDPos, jsonObj, "dD", "Positive", &ddpos, ui->dDPosComboBox);
    getSetParams(&dDNeg, jsonObj, "dD", "Negative", &ddneg, ui->dDNegComboBox);
    getSetParams(&eTPos, jsonObj, "eT", "Positive", &etpos, ui->eTPosComboBox);
    getSetParams(&eTNeg, jsonObj, "eT", "Negative", &etneg, ui->eTNegComboBox);
    getSetParams(&vRPos, jsonObj, "vR", "Positive", &vrpos, ui->vRPosComboBox);
    getSetParams(&vRNeg, jsonObj, "vR", "Negative", &vrneg, ui->vRNegComboBox);
    getSetParams(&vRPosLow, jsonObj, "vR", "PosLow", &vrposlow, ui->vRPosLowComboBox);
    getSetParams(&vRNegLow, jsonObj, "vR", "NegLow", &vrneglow, ui->vRNegLowComboBox);
    getSetParams(&vWPos, jsonObj, "vW", "Positive", &vwpos, ui->vWPosComboBox);
    getSetParams(&vWNeg, jsonObj, "vW", "Negative", &vwneg, ui->vWNegComboBox);
}

void RobotControl::getSetParams(Variable *var, QJsonObject jsonObj_, std::string id, std::string param, QVector<QLineEdit*>* pos, QComboBox *combo)
{
    QJsonObject varObj = jsonObj_[id.c_str()].toObject()[param.c_str()].toObject();
    int shape = varObj["shape"].toInt();
    QVector<float> params;
    QJsonArray params_arr = varObj["param"].toArray();

    for(int i=0; i<params_arr.size(); i++)
    {
        params.append((float)params_arr[i].toDouble());
    }

    setParam(var, param, shape, params, params.length());

    for(int i = 0; i<params.length(); i++)
    {
        pos->at(i)->setText(QString::number(params[i]));
    }
//    qDebug() << shape;
    combo->setCurrentIndex(var->shape);
}




void RobotControl::on_applyButton_clicked()
{
    setParam(&eDPos, "Positive", ui->edPosComboBox, edpos, edpos.length());
    setParam(&eDNeg, "Negative", ui->edNegComboBox, edneg, edneg.length());
    setParam(&dDPos, "Positive", ui->dDPosComboBox, ddpos, ddpos.length());
    setParam(&eDPos, "Negative", ui->dDNegComboBox, ddneg, ddneg.length());
    setParam(&eTPos, "Positive", ui->eTPosComboBox, etpos, etpos.length());
    setParam(&eTNeg, "Negative", ui->eTNegComboBox, etneg, etneg.length());
    setParam(&vRPos, "Positive", ui->vRPosComboBox, vrpos, vrpos.length());
    setParam(&vRNeg, "Negative", ui->vRNegComboBox, vrneg, vrneg.length());
    setParam(&vRPosLow, "Positive", ui->vRPosLowComboBox, vrposlow, vrposlow.length());
    setParam(&vRNegLow, "Negative", ui->vRNegLowComboBox, vrneglow, vrneglow.length());
    setParam(&vWPos, "Positive", ui->vWPosComboBox, vwpos, vwpos.length());
    setParam(&vWNeg, "Negative", ui->vWNegComboBox, vwneg, vwneg.length());
}
