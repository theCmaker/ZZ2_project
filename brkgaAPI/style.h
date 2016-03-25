#ifndef STYLE_H
#define STYLE_H

/*!
 * \file style.h
 * \brief Solutions graph style
 * \author Pierre-Loup Pissavy
 */

#include <vector>
#include <string>

/*!
 * \brief style of a graph
 */
class Style
{
    public:
        Style();    //!< Constructor
        ~Style();   //!< Destructor

        bool has_grid() const;  //!< Get the grid display status
        void setHas_grid(bool); //!< Set the grid display status

        bool has_axes() const;  //!< Get the axes display status
        void setHas_axes(bool); //!< Set the axes display status

        bool has_axes_labels() const;   //!< Get the axes' labels display status
        void setHas_axes_labels(bool);  //!< Set the axes' labels display status

        float x_step() const;           //!< Grid x step
        void setX_step(float x_step);   //!< Set grid x step

        float y_step() const;           //!< Grid y step
        void setY_step(float y_step);   //!< Set grid y step

        float y_max() const;            //!< Max y value
        void setY_max(float y_max);     //!< Set max y value

        float y_min() const;            //!< Min y value
        void setY_min(float y_min);     //!< Set min y value

        float x_max() const;            //!< Max x value
        void setX_max(float x_max);     //!< Set max x value

        float x_min() const;            //!< Min x value
        void setX_min(float x_min);     //!< Set min x value

        std::vector<double> x_ticks() const;                //!< Values of the x ticks corresponding following x step
        void setX_ticks(const std::vector<double> x_ticks); //!< Set the values of the x ticks corresponding following x step

        std::vector<double> y_ticks() const;                //!< Values of the y ticks corresponding following y step
        void setY_ticks(const std::vector<double> y_ticks); //!< Set the values of the y ticks corresponding following y step

        float width() const;            //!< Graph width
        void setWidth(float);           //!< Set graph width

        float height() const;           //!< Graph height
        void setHeight(float);          //!< Set graph height

        double aspectRatio() const;     //!< Aspect ratio
        void setAspectRatio(double);    //!< Set aspect ratio

        int selected_front() const;     //!< Id of the selected front
        void setSelected_front(int);    //!< Set the id of the selected front

private:
        bool has_grid_;         //!< Grid display status
        bool has_axes_;         //!< Axes display status
        bool has_axes_labels_;  //!< Axes' labels display status

        float x_step_;          //!< Grid x step
        float y_step_;          //!< Grid y step
        float width_;           //!< Graph width
        float height_;          //!< Graph height
        double aspect_ratio_;   //!< Graph aspect ratio
        std::vector<double> x_ticks_;   //!< x axis ticks values
        std::vector<double> y_ticks_;   //!< y axis ticks values

        int selected_front_;    //!< Id of the selected front
        float x_min_;           //!< Lowest x value
        float x_max_;           //!< Highest x value
        float y_min_;           //!< Lowest y value
        float y_max_;           //!< Highest y value
};

#endif // STYLE_H
