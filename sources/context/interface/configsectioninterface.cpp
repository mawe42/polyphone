#include "configsectioninterface.h"
#include "ui_configsectioninterface.h"
#include "contextmanager.h"
#include <QColorDialog>
#include <QMessageBox>

ConfigSectionInterface::ConfigSectionInterface(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConfigSectionInterface)
{
    ui->setupUi(this);

    ui->labelRestart->setStyleSheet("QLabel{color:" + this->palette().color(QPalette::BrightText).name() + "}");
    ui->labelRestart->hide();

    // Permanent initialization
    this->initComboLanguage();

    // Populate color themes and select the current one
    ui->comboColorTheme->blockSignals(true);
    ContextManager::theme()->populateCombobox(ui->comboColorTheme);
    fillColors();
    ui->comboColorTheme->blockSignals(false);
}

ConfigSectionInterface::~ConfigSectionInterface()
{
    delete ui;
}

bool caseInsensitiveLessThan(const QString &s1, const QString &s2)
{
    return s1.toLower() < s2.toLower();
}

void ConfigSectionInterface::initComboLanguage()
{
    // Load the different languages
    ui->comboLangue->blockSignals(true);
    QMap<QString, QString> languages = ContextManager::translation()->getLanguages();
    QStringList languageNames = languages.values();
    qSort(languageNames.begin(), languageNames.end(), caseInsensitiveLessThan);

    foreach (QString languageName, languageNames)
        ui->comboLangue->addItem(languageName, languages.key(languageName));

    // Selection of the current language
    QString locale = QLocale::system().name().section('_', 0, 0);
    locale = ContextManager::configuration()->getValue(ConfManager::SECTION_NONE, "language", locale).toString();
    bool found = false;
    for (int i = 0; i < ui->comboLangue->count(); i++)
    {
        if (ui->comboLangue->itemData(i).toString() == locale)
        {
            found = true;
            ui->comboLangue->setCurrentIndex(i);
            break;
        }
    }

    // If not found, english is the default
    if (!found)
    {
        for (int i = 0; i < ui->comboLangue->count(); i++)
        {
            if (ui->comboLangue->itemData(i).toString() == "en")
            {
                ui->comboLangue->setCurrentIndex(i);
                break;
            }
        }
    }

    ui->comboLangue->blockSignals(false);
}

void ConfigSectionInterface::initialize()
{
    ui->comboKeyName->blockSignals(true);
    ui->comboKeyName->setCurrentIndex((int)ContextManager::keyName()->getNameMiddleC());
    ui->comboKeyName->blockSignals(false);
}

void ConfigSectionInterface::fillColors()
{
    QString styleStart = "QPushButton{border: 1px solid #888; background-color: ";
    ui->pushColorWindowBackground->setStyleSheet(
                styleStart + ContextManager::theme()->getColor(ThemeManager::WINDOW_BACKGROUND).name() + ";}");
    ui->pushColorWindowText->setStyleSheet(
                styleStart + ContextManager::theme()->getColor(ThemeManager::WINDOW_TEXT).name() + ";}");
    ui->pushColorButtonBackground->setStyleSheet(
                styleStart + ContextManager::theme()->getColor(ThemeManager::BUTTON_BACKGROUND).name() + ";}");
    ui->pushColorButtonText->setStyleSheet(
                styleStart + ContextManager::theme()->getColor(ThemeManager::BUTTON_TEXT).name() + ";}");
    ui->pushColorSelectionBackground->setStyleSheet(
                styleStart + ContextManager::theme()->getColor(ThemeManager::HIGHLIGHTED_BACKGROUND).name() + ";}");
    ui->pushColorSelectionText->setStyleSheet(
                styleStart + ContextManager::theme()->getColor(ThemeManager::HIGHLIGHTED_TEXT).name() + ";}");
    ui->pushColorListBackground->setStyleSheet(
                styleStart + ContextManager::theme()->getColor(ThemeManager::LIST_BACKGROUND).name() + ";}");
    ui->pushColorListAlternativeBackground->setStyleSheet(
                styleStart + ContextManager::theme()->getColor(ThemeManager::LIST_ALTERNATIVE_BACKGROUND).name() + ";}");
    ui->pushColorListText->setStyleSheet(
                styleStart + ContextManager::theme()->getColor(ThemeManager::LIST_TEXT).name() + ";}");
    ContextManager::theme()->selectIndex(ui->comboColorTheme);
}

void ConfigSectionInterface::on_comboColorTheme_currentIndexChanged(int index)
{
    if (index == 0) {
        ContextManager::theme()->resetTheme();
        fillColors();
        ui->labelRestart->show();
    } else {
        int themeIndex = ui->comboColorTheme->itemData(index).toInt();
        if (themeIndex != -1) {
            ContextManager::theme()->applyTheme(themeIndex);
            fillColors();
            ui->labelRestart->show();
        }
    }
}

void ConfigSectionInterface::on_pushColorWindowBackground_clicked()
{
    QColor color = QColorDialog::getColor(ContextManager::theme()->getColor(ThemeManager::WINDOW_BACKGROUND),
                                          this, trUtf8("Choisissez une couleur"));
    if (color.isValid())
    {
        ContextManager::theme()->setColor(ThemeManager::WINDOW_BACKGROUND, color);
        this->fillColors();
        ui->labelRestart->show();
    }
}

void ConfigSectionInterface::on_pushColorButtonBackground_clicked()
{
    QColor color = QColorDialog::getColor(ContextManager::theme()->getColor(ThemeManager::BUTTON_BACKGROUND),
                                          this, trUtf8("Choisissez une couleur"));
    if (color.isValid())
    {
        ContextManager::theme()->setColor(ThemeManager::BUTTON_BACKGROUND, color);
        this->fillColors();
        ui->labelRestart->show();
    }
}

void ConfigSectionInterface::on_pushColorSelectionBackground_clicked()
{
    QColor color = QColorDialog::getColor(ContextManager::theme()->getColor(ThemeManager::HIGHLIGHTED_BACKGROUND),
                                          this, trUtf8("Choisissez une couleur"));
    if (color.isValid())
    {
        ContextManager::theme()->setColor(ThemeManager::HIGHLIGHTED_BACKGROUND, color);
        this->fillColors();
        ui->labelRestart->show();
    }
}

void ConfigSectionInterface::on_pushColorListBackground_clicked()
{
    QColor color = QColorDialog::getColor(ContextManager::theme()->getColor(ThemeManager::LIST_BACKGROUND),
                                          this, trUtf8("Choisissez une couleur"));
    if (color.isValid())
    {
        ContextManager::theme()->setColor(ThemeManager::LIST_BACKGROUND, color);
        this->fillColors();
        ui->labelRestart->show();
    }
}

void ConfigSectionInterface::on_pushColorListAlternativeBackground_clicked()
{
    QColor color = QColorDialog::getColor(ContextManager::theme()->getColor(ThemeManager::LIST_ALTERNATIVE_BACKGROUND),
                                          this, trUtf8("Choisissez une couleur"));
    if (color.isValid())
    {
        ContextManager::theme()->setColor(ThemeManager::LIST_ALTERNATIVE_BACKGROUND, color);
        this->fillColors();
        ui->labelRestart->show();
    }
}

void ConfigSectionInterface::on_pushColorWindowText_clicked()
{
    QColor color = QColorDialog::getColor(ContextManager::theme()->getColor(ThemeManager::WINDOW_TEXT),
                                          this, trUtf8("Choisissez une couleur"));
    if (color.isValid())
    {
        ContextManager::theme()->setColor(ThemeManager::WINDOW_TEXT, color);
        this->fillColors();
        ui->labelRestart->show();
    }
}

void ConfigSectionInterface::on_pushColorButtonText_clicked()
{
    QColor color = QColorDialog::getColor(ContextManager::theme()->getColor(ThemeManager::BUTTON_TEXT),
                                          this, trUtf8("Choisissez une couleur"));
    if (color.isValid())
    {
        ContextManager::theme()->setColor(ThemeManager::BUTTON_TEXT, color);
        this->fillColors();
        ui->labelRestart->show();
    }
}

void ConfigSectionInterface::on_pushColorSelectionText_clicked()
{
    QColor color = QColorDialog::getColor(ContextManager::theme()->getColor(ThemeManager::HIGHLIGHTED_TEXT),
                                          this, trUtf8("Choisissez une couleur"));
    if (color.isValid())
    {
        ContextManager::theme()->setColor(ThemeManager::HIGHLIGHTED_TEXT, color);
        this->fillColors();
        ui->labelRestart->show();
    }
}

void ConfigSectionInterface::on_pushColorListText_clicked()
{
    QColor color = QColorDialog::getColor(ContextManager::theme()->getColor(ThemeManager::LIST_TEXT),
                                          this, trUtf8("Choisissez une couleur"));
    if (color.isValid())
    {
        ContextManager::theme()->setColor(ThemeManager::LIST_TEXT, color);
        this->fillColors();
        ui->labelRestart->show();
    }
}

void ConfigSectionInterface::on_comboLangue_currentIndexChanged(int index)
{
    ContextManager::configuration()->setValue(ConfManager::SECTION_NONE, "language", ui->comboLangue->itemData(index));
    ui->labelRestart->show();
}

void ConfigSectionInterface::on_comboKeyName_currentIndexChanged(int index)
{
    ContextManager::keyName()->setMiddleKey((KeyNameManager::NameMiddleC)index);
    ui->labelRestart->show();
}