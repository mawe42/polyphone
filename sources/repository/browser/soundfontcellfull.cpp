#include "soundfontcellfull.h"
#include "ui_soundfontcellfull.h"
#include "soundfontinformation.h"
#include "repositorymanager.h"
#include "contextmanager.h"

SoundfontCellFull::IconContainer * SoundfontCellFull::s_icons = NULL;

SoundfontCellFull::SoundfontCellFull(SoundfontInformation* soundfontInfo, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SoundfontCellFull),
    _active(true)
{
    ui->setupUi(this);

    if (s_icons == NULL)
        s_icons = new IconContainer();

    // Title
    ui->labelTitle->setTextToElide(soundfontInfo->getTitle());

    // Numbers
    ui->labelCommentNumber->setText(QString::number(soundfontInfo->getCommentNumber()));
    ui->labelDownloadNumber->setText(QString::number(soundfontInfo->getDownloadNumber()));
    ui->widgetStars->setScore(soundfontInfo->getRating());

    // Author
    _authorTextNoColor = soundfontInfo->getAuthor();
    _authorTextNoColor = "<a style=\"text-decoration:none;color:%0;\" href=\"" + _authorTextNoColor + "\">" + _authorTextNoColor + "</a>";

    // Date
    ui->labelDate->setText(soundfontInfo->getDateTime().toString(Qt::SystemLocaleShortDate));

    // License
    _licenseTextNoColor = "<a style=\"text-decoration:none;color:%0;\" href=\"" +
            RepositoryManager::getInstance()->getLicenseLink(soundfontInfo->getLicense()) +
            "\">" + RepositoryManager::getInstance()->getLicenseLabel(soundfontInfo->getLicense()) + "</a>";

    // Attributes
    connect(ui->line3, SIGNAL(itemClicked(SoundfontFilter*)), this, SIGNAL(itemClicked(SoundfontFilter*)));
    ui->line3->addCategory(soundfontInfo->getCategoryId());
    foreach (SoundfontInformation::Property key, soundfontInfo->getProperties().keys())
        foreach (QString value, soundfontInfo->getProperties()[key])
            ui->line3->addProperty(key, value);
    foreach (QString tag, soundfontInfo->getTags())
        ui->line3->addTag(tag);

    // Style when the cell is activated and when it's not
    QColor buttonBackground = ContextManager::theme()->getColor(ThemeManager::HIGHLIGHTED_BACKGROUND);
    QColor buttonText = ContextManager::theme()->getColor(ThemeManager::HIGHLIGHTED_TEXT);
    QColor buttonBackgroundHover = ContextManager::theme()->getColor(ThemeManager::HIGHLIGHTED_BACKGROUND, ThemeManager::HOVERED);
    QColor buttonBackgroundHover2 = ContextManager::theme()->getColor(ThemeManager::HIGHLIGHTED_TEXT, ThemeManager::HOVERED);
    QString tmp = "QPushButton{background-color:%0; color:%1;border:0px;padding:5px;border-radius:4px;}\
            QPushButton:hover{ background-color:%2;}QLabel#labelAuthor{color:%0;}";
    _normalStyleSheet = tmp.arg(buttonBackground.name()).arg(buttonText.name()).arg(buttonBackgroundHover.name());
    _activeStyleSheet = tmp.arg(buttonText.name()).arg(buttonBackground.name()).arg(buttonBackgroundHover2.name()) +
            "QLabel{color:" + ContextManager::theme()->getColor(ThemeManager::HIGHLIGHTED_TEXT).name() + ";}";

    // Initialize the style
    this->setActive(false);
}

SoundfontCellFull::~SoundfontCellFull()
{
    delete ui;
}

void SoundfontCellFull::setActive(bool isActive)
{
    if (isActive && !_active)
    {
        // Colors
        this->setStyleSheet(_activeStyleSheet);

        // Icons
        ui->iconComment->setPixmap(s_icons->_commentIconSelected);
        ui->iconDownload->setPixmap(s_icons->_downloadIconSelected);
        ui->iconAuthor->setPixmap(s_icons->_userIconSelected);
        ui->iconDate->setPixmap(s_icons->_dateIconSelected);
        ui->iconLicense->setPixmap(s_icons->_copyrightIconSelected);

        // Author and license texts
        QString linkColor = ContextManager::theme()->getColor(ThemeManager::HIGHLIGHTED_TEXT).name();
        ui->labelAuthor->setText(QString(_authorTextNoColor).arg(linkColor));
        ui->labelLicense->setText(QString(_licenseTextNoColor).arg(linkColor));
    }
    else if (!isActive && _active)
    {
        // Colors
        this->setStyleSheet(_normalStyleSheet);

        // Icons
        ui->iconComment->setPixmap(s_icons->_commentIcon);
        ui->iconDownload->setPixmap(s_icons->_downloadIcon);
        ui->iconAuthor->setPixmap(s_icons->_userIcon);
        ui->iconDate->setPixmap(s_icons->_dateIcon);
        ui->iconLicense->setPixmap(s_icons->_copyrightIcon);

        // Author and license texts
        QString linkColor = ContextManager::theme()->getColor(ThemeManager::HIGHLIGHTED_BACKGROUND).name();
        ui->labelAuthor->setText(QString(_authorTextNoColor).arg(linkColor));
        ui->labelLicense->setText(QString(_licenseTextNoColor).arg(linkColor));
    }
    _active = isActive;

    // Stars
    ui->widgetStars->setActive(isActive);
}

SoundfontCellFull::IconContainer::IconContainer()
{
    // Base icons
    _commentIcon = ContextManager::theme()->getColoredSvg(":/icons/comment.svg", QSize(24, 24), ThemeManager::ColorType::LIST_TEXT);
    _downloadIcon = ContextManager::theme()->getColoredSvg(":/icons/download.svg", QSize(24, 24), ThemeManager::ColorType::LIST_TEXT);
    _userIcon = ContextManager::theme()->getColoredSvg(":/icons/user.svg", QSize(16, 16), ThemeManager::ColorType::LIST_TEXT);
    _dateIcon = ContextManager::theme()->getColoredSvg(":/icons/calendar.svg", QSize(16, 16), ThemeManager::ColorType::LIST_TEXT);
    _copyrightIcon = ContextManager::theme()->getColoredSvg(":/icons/copyright.svg", QSize(16, 16), ThemeManager::ColorType::LIST_TEXT);

    // Icons under selection
    _commentIconSelected = ContextManager::theme()->getColoredSvg(":/icons/comment.svg", QSize(24, 24), ThemeManager::ColorType::HIGHLIGHTED_TEXT);
    _downloadIconSelected = ContextManager::theme()->getColoredSvg(":/icons/download.svg", QSize(24, 24), ThemeManager::ColorType::HIGHLIGHTED_TEXT);
    _userIconSelected = ContextManager::theme()->getColoredSvg(":/icons/user.svg", QSize(16, 16), ThemeManager::ColorType::HIGHLIGHTED_TEXT);
    _dateIconSelected = ContextManager::theme()->getColoredSvg(":/icons/calendar.svg", QSize(16, 16), ThemeManager::ColorType::HIGHLIGHTED_TEXT);
    _copyrightIconSelected = ContextManager::theme()->getColoredSvg(":/icons/copyright.svg", QSize(16, 16), ThemeManager::ColorType::HIGHLIGHTED_TEXT);
}

void SoundfontCellFull::on_labelAuthor_linkActivated(const QString &link)
{
    SoundfontFilter * filter = new SoundfontFilter();
    filter->setSearchText(QString("Author:\"%0\"").arg(link));
    emit(itemClicked(filter));
}

int SoundfontCellFull::heightForWidth(int width) const
{
    return 30 + ui->line1->height() + ui->line2->height() + ui->line3->heightForWidth(width - 18);
}

bool SoundfontCellFull::hasHeightForWidth() const
{
    return true;
}