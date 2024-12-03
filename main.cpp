#include <Magick++.h>
#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QPushButton>
#include <QWidget>
#include <QFileInfo>
#include <filesystem>

std::string CompressionTypeToString(Magick::CompressionType compression) {
    switch (compression) {
    case Magick::NoCompression: return "No Compression";
    case Magick::BZipCompression: return "BZip Compression";
    case Magick::FaxCompression: return "Fax Compression";
    case Magick::Group4Compression: return "Group 4 Compression";
    case Magick::JPEGCompression: return "JPEG Compression";
    case Magick::LosslessJPEGCompression: return "Lossless JPEG Compression";
    case Magick::LZWCompression: return "LZW Compression";
    case Magick::RLECompression: return "RLE Compression";
    case Magick::ZipCompression: return "Zip Compression";
    default: return "Unknown Compression";
    }
}

void ExtractImageInfo(const std::string& filePath, QTextEdit* outputWidget) {
    Magick::Image image;
    try {
        image.read(filePath);
        std::string filaName = std::filesystem::path(filePath).filename().string();

        QString fileInfo = QString("Имя файла: %1\n").arg(QString::fromStdString(filaName));

        QString fileFormat = QString::fromStdString(image.magick());
        fileInfo += QString("Формат: %1\n").arg(fileFormat);

        int width = image.columns();
        int height = image.rows();
        fileInfo += QString("Размер изображения: %1x%2 пикселей\n").arg(width).arg(height);

        double resolutionX = image.xResolution();
        double resolutionY = image.yResolution();
        if(resolutionX == 0 && resolutionY == 0 && fileFormat == "PNG"){
            resolutionX = resolutionY = 76;
        }
        fileInfo += QString("Разрешение: %1x%2 DPI\n").arg(resolutionX).arg(resolutionY);

        int depth = image.depth();
        fileInfo += QString("Глубина цвета: %1 бит\n").arg(depth);

        Magick::CompressionType compression = image.compressType();
        fileInfo += QString("Сжатие: %1\n").arg(QString::fromStdString(CompressionTypeToString(compression)));

        QFileInfo fileInfoObj(QString::fromStdString(filePath));
        qint64 fileSize = fileInfoObj.size();
        fileInfo += QString("Размер файла: %1 байт\n").arg(fileSize);

        outputWidget->append(fileInfo);
    }
    catch (Magick::Exception& e) {
        QMessageBox::critical(nullptr, "Ошибка", e.what());
    }
}

class MainWindow : public QWidget {
public:
    MainWindow(QWidget* parent = nullptr) : QWidget(parent) {
        setWindowTitle("Информация об изображениях");
        resize(600, 400);

        QVBoxLayout* layout = new QVBoxLayout(this);

        outputWidget = new QTextEdit(this);
        outputWidget->setReadOnly(true);
        layout->addWidget(outputWidget);

        QPushButton* selectFolderButton = new QPushButton("Выбрать папку", this);
        layout->addWidget(selectFolderButton);

        connect(selectFolderButton, &QPushButton::clicked, this, &MainWindow::onSelectFolder);
    }

private slots:
    void onSelectFolder() {
        // Открываем диалог для выбора папки
        QString folderPath = QFileDialog::getExistingDirectory(this, "Выберите папку с изображениями");

        if (folderPath.isEmpty()) {
            return;
        }

        QDir folder(folderPath);
        QStringList filters;
        filters << "*.jpg" << "*.jpeg" << "*.png" << "*.bmp" << "*.gif" << "*.tif";
        folder.setNameFilters(filters);
        QStringList files = folder.entryList();

        outputWidget->clear();

        for (const QString& file : files) {
            QString filePath = folder.absoluteFilePath(file);
            ExtractImageInfo(filePath.toStdString(), outputWidget);
        }
    }

private:
    QTextEdit* outputWidget;
};

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    Magick::InitializeMagick(nullptr);

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
