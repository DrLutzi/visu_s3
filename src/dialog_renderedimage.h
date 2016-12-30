#ifndef DIALOG_RENDEREDIMAGE_H
#define DIALOG_RENDEREDIMAGE_H

#include <QDialog>

namespace Ui {
class Dialog_RenderedImage;
}

class Dialog_RenderedImage : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_RenderedImage(QWidget *parent = 0);
    ~Dialog_RenderedImage();

    void setImage(const QImage *image);

private:
    Ui::Dialog_RenderedImage *ui;
};

#endif // DIALOG_RENDEREDIMAGE_H
