#include "style.h"

Style::Style() {}

Style::~Style() {}

bool Style::has_grid() const
{
    return has_grid_;
}

void Style::setHas_grid(bool has_grid)
{
    has_grid_ = has_grid;
}
bool Style::has_axes() const
{
    return has_axes_;
}

void Style::setHas_axes(bool has_axes)
{
    has_axes_ = has_axes;
}
bool Style::has_axes_labels() const
{
    return has_axes_labels_;
}

void Style::setHas_axes_labels(bool has_axes_labels)
{
    has_axes_labels_ = has_axes_labels;
}
float Style::x_step() const
{
    return x_step_;
}

void Style::setX_step(float x_step)
{
    x_step_ = x_step;
}
float Style::y_step() const
{
    return y_step_;
}

void Style::setY_step(float y_step)
{
    y_step_ = y_step;
}





