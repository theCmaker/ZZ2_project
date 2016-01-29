#ifndef STYLE_H
#define STYLE_H

class Style
{
    public:
        Style();
        ~Style();

        bool has_grid() const;
        void setHas_grid(bool has_grid);

        bool has_axes() const;
        void setHas_axes(bool has_axes);

        bool has_axes_labels() const;
        void setHas_axes_labels(bool has_axes_labels);

        float x_step() const;
        void setX_step(float x_step);

        float y_step() const;
        void setY_step(float y_step);

private:
        bool has_grid_;
        bool has_axes_;
        bool has_axes_labels_;

        float x_step_;
        float y_step_;
};

#endif // STYLE_H
