#include "dialog_renderedimage.h"
#include "ui_dialog_renderedimage.h"

Dialog_RenderedImage::Dialog_RenderedImage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_RenderedImage)
{
    ui->setupUi(this);
}

Dialog_RenderedImage::~Dialog_RenderedImage()
{
    delete ui;
}

void Dialog_RenderedImage::setImage(const QImage *image)
{
    ui->label_image->setPixmap(QPixmap::fromImage(*image));
}
