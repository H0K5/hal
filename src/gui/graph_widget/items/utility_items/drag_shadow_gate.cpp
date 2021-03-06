#include "graph_widget/items/utility_items/drag_shadow_gate.h"

#include "graph_widget/items/graphics_gate.h"

#include <assert.h>

#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include "QPainter"
#include "QPaintEvent"

QPen drag_shadow_gate::s_pen;
qreal drag_shadow_gate::s_lod;

QColor drag_shadow_gate::s_color_pen[3];
QColor drag_shadow_gate::s_color_solid[3];
QColor drag_shadow_gate::s_color_translucent[3];

void drag_shadow_gate::load_settings()
{
    s_color_pen[static_cast<int>(drag_cue::rejected)] = QColor(166, 31, 31, 255);
    s_color_pen[static_cast<int>(drag_cue::movable)] = QColor(48, 172, 79, 255);
    s_color_pen[static_cast<int>(drag_cue::swappable)] = QColor(37, 97, 176, 255);

    s_color_solid[static_cast<int>(drag_cue::rejected)] = QColor(166, 31, 31, 200);
    s_color_solid[static_cast<int>(drag_cue::movable)] = QColor(48, 172, 79, 200);
    s_color_solid[static_cast<int>(drag_cue::swappable)] = QColor(37, 97, 176, 200);

    s_color_translucent[static_cast<int>(drag_cue::rejected)] = QColor(166, 31, 31, 150);
    s_color_translucent[static_cast<int>(drag_cue::movable)] = QColor(48, 172, 79, 150);
    s_color_translucent[static_cast<int>(drag_cue::swappable)] = QColor(37, 97, 176, 150);

    s_pen.setCosmetic(true);
    s_pen.setJoinStyle(Qt::MiterJoin);
}

drag_shadow_gate::drag_shadow_gate() : QGraphicsObject()
{
    hide();

    setAcceptedMouseButtons(0);
    m_width = 100;
    m_height = 100;
}

void drag_shadow_gate::start(const QPointF& posF, const QSizeF& sizeF)
{
    setPos(posF);
    set_width(sizeF.width());
    set_height(sizeF.height());
    setZValue(1);
    show();
}

void drag_shadow_gate::stop()
{
    hide();
}

qreal drag_shadow_gate::width() const
{
    return m_width;
}

qreal drag_shadow_gate::height() const
{
    return m_height;
}

QSizeF drag_shadow_gate::size() const
{
    return QSizeF(width(), height());
}

void drag_shadow_gate::set_width(const qreal width)
{
    m_width = width;
}

void drag_shadow_gate::set_height(const qreal height)
{
    m_height = height;
}

void drag_shadow_gate::set_lod(const qreal lod)
{
    s_lod = lod;
}

void drag_shadow_gate::set_visual_cue(const drag_cue cue)
{
    m_cue = cue;
    update();
}

void drag_shadow_gate::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    int color_index = static_cast<int>(m_cue);
    assert(color_index <= 3);

    // painter->save();
    s_pen.setColor(s_color_pen[color_index]);
    painter->setPen(s_pen);

    if (s_lod < 0.5)
    {
        painter->fillRect(QRectF(0, 0, m_width, m_height), s_color_solid[color_index]);
    }
    else
    {
        painter->drawRect(0, 0, m_width, m_height);
        painter->fillRect(QRectF(0, 0, m_width, m_height), s_color_translucent[color_index]);
    }
}

QRectF drag_shadow_gate::boundingRect() const
{
    return QRectF(0, 0, m_width, m_height);
}

QPainterPath drag_shadow_gate::shape() const
{
    QPainterPath path;
    path.addRect(QRectF(0, 0, m_width, m_height));
    return path;
}
