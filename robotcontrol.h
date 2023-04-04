#ifndef ROBOTCONTROL_H
#define ROBOTCONTROL_H

#include <QMainWindow>
#include <QVector>
#include <iostream>
#include <QDebug>
#include <QString>
#include <QComboBox>
#include <QLineEdit>
#include <QFile>
#include <QFileDialog>
#include <QString>
#include <QComboBox>

QT_BEGIN_NAMESPACE
namespace Ui { class RobotControl; }
QT_END_NAMESPACE

class RobotControl : public QMainWindow
{
    Q_OBJECT

public:

    struct Vals{
        QVector<float> values;
    };

    struct Variable
    {
        std::string id;
        QVector<float> params;
        int shape;
    };

    RobotControl(QWidget *parent = nullptr);
    ~RobotControl();

    void setParam(Variable *x, std::string ids, QComboBox *cb, QVector<QLineEdit*> line, int length);
    void setParam(Variable *x, std::string ids, int shape_, QVector<float> par, int length);
    void setLine(QVector<QLineEdit*>* pos, QVector<QLineEdit*>* pos_);
    void varArray(Variable x);
    void getDefault();
    void getSetParams(Variable *var, QJsonObject jsonObj_, std::string id, std::string param, QVector<QLineEdit*>*pos, QComboBox *combo);

private slots:

    void on_applyButton_clicked();

private:
    Ui::RobotControl *ui;
    Vals val_srv;

    Variable eDPos;
    Variable eDNeg;
    Variable dDPos;
    Variable dDNeg;
    Variable eTPos;
    Variable eTNeg;
    Variable vRPos;
    Variable vRNeg;
    Variable vRPosLow;
    Variable vRNegLow;
    Variable vWPos;
    Variable vWNeg;

    QVector<QLineEdit*> edpos;
    QVector<QLineEdit*> edneg;
    QVector<QLineEdit*> ddpos;
    QVector<QLineEdit*> ddneg;
    QVector<QLineEdit*> etpos;
    QVector<QLineEdit*> etneg;
    QVector<QLineEdit*> vrpos;
    QVector<QLineEdit*> vrneg;
    QVector<QLineEdit*> vrposlow;
    QVector<QLineEdit*> vrneglow;
    QVector<QLineEdit*> vwpos;
    QVector<QLineEdit*> vwneg;

    QVector<Variable> var_arr;
};
#endif // ROBOTCONTROL_H
