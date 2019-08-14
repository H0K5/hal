#include "graph_widget/items/nets/arrow_separated_net.h"

#include "graph_widget/graph_widget_constants.h"

#include <QPainter>
#include <QPen>
#include <QStyleOptionGraphicsItem>

qreal arrow_separated_net::s_wire_length;
qreal arrow_separated_net::s_circle_offset;
qreal arrow_separated_net::s_radius;

QBrush arrow_separated_net::s_brush;

void arrow_separated_net::load_settings()
{
    s_wire_length   = 26;
    s_circle_offset = 0;
    s_radius        = 3;

    s_brush.setStyle(Qt::SolidPattern);
    s_pen.setColor(QColor(160, 160, 160)); // USE STYLESHEETS
}

arrow_separated_net::arrow_separated_net(const std::shared_ptr<const net> n) : separated_graphics_net(n)
{

}

void arrow_separated_net::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(widget);

    if (s_lod < graph_widget_constants::global_net_min_lod)
        return;

    QColor color = (option->state & QStyle::State_Selected) ? s_selection_color : m_color;
    color.setAlphaF(s_alpha);

    s_pen.setColor(color);
    s_brush.setColor(color);
    painter->setPen(s_pen);
    painter->setBrush(s_brush);

    if (m_draw_output)
    {
        painter->drawLine(QPointF(0, 0), QPointF(s_wire_length, 0));
        painter->setRenderHint(QPainter::Antialiasing, true);
        painter->drawEllipse(QPointF(s_wire_length + s_circle_offset + s_radius, 0), s_radius, s_radius);
        painter->setRenderHint(QPainter::Antialiasing, false);
    }

    for (const QPointF& position : m_input_wires)
    {
        QPointF to(position.x() - s_wire_length, position.y());
        painter->drawLine(position, to);
        painter->setRenderHint(QPainter::Antialiasing, true);
        painter->drawEllipse(QPointF(to.x() - s_circle_offset - s_radius, to.y()), s_radius, s_radius);
        painter->setRenderHint(QPainter::Antialiasing, false);
    }

#ifdef HAL_DEBUG_GUI_GRAPHICS
    s_pen.setColor(Qt::green);
    painter->setPen(s_pen);
    painter->drawPath(m_shape);
#endif

    painter->setBrush(QBrush());
}

void arrow_separated_net::set_visuals(const graphics_net::visuals& v)
{
    setVisible(v.visible);
    m_color = v.color;
    m_line_style = v.style;
}

void arrow_separated_net::add_output()
{
    if (m_draw_output)
        return;

    m_draw_output = true;

    m_shape.moveTo(QPointF(0, -s_stroke_width / 2));
    m_shape.lineTo(QPointF(m_shape.currentPosition().x() + s_wire_length + s_circle_offset, m_shape.currentPosition().y()));
    m_shape.lineTo(QPointF(m_shape.currentPosition().x(), m_shape.currentPosition().y() - s_radius + s_stroke_width / 2));
    m_shape.lineTo(QPointF(m_shape.currentPosition().x() + s_radius * 2, m_shape.currentPosition().y()));
    m_shape.lineTo(QPointF(m_shape.currentPosition().x(), m_shape.currentPosition().y() + s_radius * 2));
    m_shape.lineTo(QPointF(m_shape.currentPosition().x() - s_radius * 2, m_shape.currentPosition().y()));
    m_shape.lineTo(QPointF(m_shape.currentPosition().x(), m_shape.currentPosition().y() - s_radius + s_stroke_width / 2));
    m_shape.lineTo(QPointF(m_shape.currentPosition().x() - s_wire_length - s_circle_offset, m_shape.currentPosition().y()));
    m_shape.closeSubpath();
}

void arrow_separated_net::add_input(const QPointF& scene_position)
{
    QPointF mapped_position = mapFromScene(scene_position);
    m_input_wires.append(mapped_position);

    m_shape.moveTo(QPointF(mapped_position.x(), mapped_position.y() - s_stroke_width / 2));
    m_shape.lineTo(QPointF(m_shape.currentPosition().x() - s_wire_length - s_circle_offset, m_shape.currentPosition().y()));
    m_shape.lineTo(QPointF(m_shape.currentPosition().x(), m_shape.currentPosition().y() - s_radius + s_stroke_width / 2));
    m_shape.lineTo(QPointF(m_shape.currentPosition().x() - s_radius * 2, m_shape.currentPosition().y()));
    m_shape.lineTo(QPointF(m_shape.currentPosition().x(), m_shape.currentPosition().y() + s_radius * 2));
    m_shape.lineTo(QPointF(m_shape.currentPosition().x() + s_radius * 2, m_shape.currentPosition().y()));
    m_shape.lineTo(QPointF(m_shape.currentPosition().x(), m_shape.currentPosition().y() - s_radius + s_stroke_width / 2));
    m_shape.lineTo(QPointF(m_shape.currentPosition().x() + s_wire_length + s_circle_offset, m_shape.currentPosition().y()));
    m_shape.closeSubpath();
}

void arrow_separated_net::finalize()
{
    m_rect = m_shape.boundingRect();
    m_rect.adjust(-1, -1, 1, 1);
}

qreal arrow_separated_net::width() const
{
    return s_wire_length + s_circle_offset + 2 * s_radius;
}