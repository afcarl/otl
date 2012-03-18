#ifndef OTL_WINDOW_3281907590812038290178498638726509842309432
#define OTL_WINDOW_3281907590812038290178498638726509842309432

#include "otl_aug_feature.h"
#include "otl_helpers.h"
#include <fstream>
#include <exception>
#include <string>

namespace OTL {
class Window : public AugmentedFeatures {
public:

    /**
        \brief updates the internal window with the input
        \param input the input
    **/

    virtual void update(const VectorXd &input) {
//        if (initial_sample) {
//            for (unsigned int i=0; i<this->input_dim; i++) {
//                state(i) = input(i);
//            }
//        } else {
            //push back all the samples
        VectorXd temp = this->window.segment(0,this->total_window_size - this->input_dim);
            window.segment(this->input_dim, this->total_window_size-this->input_dim)
                    = temp;

            //std::cout << "....\n" << window << "...\n" << std::endl;
            //add the new one to the front
            window.segment(0, this->input_dim) = input;
//        }

        return;
    }

    /**
        \brief Gets the internal window and puts it into the features parameter
        \param features where the window gets put into.
    **/
    virtual void getFeatures(VectorXd &features) {
        features = this->window;
        return;
    }

    /**
        \brief Sets the internal window to the parameter features
        \param features a VectorXd with the features you want to set. Make sure it is the right size.
    **/
    virtual void setFeatures(const VectorXd &features) {
        if (features.rows() != this->total_window_size) {
            throw OTLException("Sorry, the feature you want to set is not the right length");
        }
        //set the features
        this->window = features;
        return;
    }

    /**
        \brief Resets the window to all zeros
    **/
    virtual void reset(void) {
        this->window = VectorXd::Zero(this->total_window_size);
        return;
    }

    /**
        \brief Saves the window features to a file
        \param filename a string specifying the filename to save to
    **/
    virtual void save(std::string filename) {

        std::ofstream out;
        try {
            out.open(filename.c_str());
        } catch (std::exception &e) {
            std::string error_msg = "Cannot open ";
            error_msg += filename + " for writing";
            throw OTLException(error_msg);
        }

        out << input_dim << std::endl;
        out << output_dim << std::endl;
        out << window_length << std::endl;
        out << total_window_size << std::endl;
        saveVectorToStream(out, window);
        out.close();
    }


    /**
        \brief Loads the window from a file
        \param filename a string specifying the filename to load from
    **/
    virtual void load(std::string filename) {
        std::ifstream in;
        try {
            in.open(filename.c_str());
        } catch (std::exception &e) {
            std::string error_msg = "Cannot open ";
            error_msg += filename + " for writing";
            throw OTLException(error_msg);
        }

        in >> this->input_dim;
        in >> this->output_dim;
        in >> this->window_length;
        in >> this->total_window_size;
        readVectorFromStream(in, this->window);
        in.close();
    }

    /**
        \brief Sets up the window.
        \param input_dim the input dimension
        \param output_dim the output dimension
        \param window_length how long is the delay window? This value must be
                strictly positive. A delay window of 1 means no windowing.
    **/
    void init(unsigned int input_dim, unsigned int output_dim, unsigned int window_length) {
        if (window_length == 0) {
            throw OTLException("You need a window bigger than 0. (Try 1 if don't want a window).");
        }
        if (input_dim == 0) {
            throw OTLException("Input dimension cannot be zero.");
        }
        if (output_dim == 0) {
            throw OTLException("Output dimension cannot be zero.");
        }

        this->input_dim = input_dim;
        this->output_dim = output_dim;
        this->window_length = window_length;
        this->total_window_size = input_dim*window_length;

        //initialise the window
        this->window = VectorXd::Zero(this->total_window_size);

        return;
    }

private:
    //Windowing parameters
    unsigned int input_dim;
    unsigned int output_dim;
    unsigned int window_length;
    unsigned int total_window_size;
    VectorXd window;
};
}

#endif