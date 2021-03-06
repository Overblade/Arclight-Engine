#include "importdialog.h"
#include "./ui_importdialog.h"



ImportDialog::ImportDialog(QWidget *parent) : QDialog(parent), ui(new Ui::ImportDialog){

    ui->setupUi(this);
    setFixedHeight(sizeHint().height());
    setFixedWidth(sizeHint().width());

    onGenerateNormalsCheckBoxChanged(Qt::Unchecked);

    connect(ui->importButton, SIGNAL(clicked()), this, SLOT(onImportAccepted()));
    connect(ui->genNormalsCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onGenerateNormalsCheckBoxChanged(int)));

}



ImportConfiguration ImportDialog::getImportConfiguration(){
    return config;
}



void ImportDialog::onGenerateNormalsCheckBoxChanged(int state){

    if(state == Qt::Checked){
        ui->faceNormalsRadioButton->setEnabled(true);
        ui->smoothNormalsRadioButton->setEnabled(true);
    }else{
        ui->faceNormalsRadioButton->setEnabled(false);
        ui->smoothNormalsRadioButton->setEnabled(false);
    }

}



void ImportDialog::onImportAccepted(){

    config.flipUVs = ui->flipUVsCheckBox->isChecked();
    config.genNormals = ui->genNormalsCheckBox->isChecked();
    config.smoothNormals = ui->smoothNormalsRadioButton->isChecked();
    config.maxWeigths = ui->weightLimitSpinBox->value();
    config.genTangents = ui->calcTangentsCheckBox->isChecked();

    accept();

}



ImportDialog::~ImportDialog(){
    delete ui;
}
