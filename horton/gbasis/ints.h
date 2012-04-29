// Horton is a Density Functional Theory program.
// Copyright (C) 2011-2012 Toon Verstraelen <Toon.Verstraelen@UGent.be>
//
// This file is part of Horton.
//
// Horton is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or (at your option) any later version.
//
// Horton is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>
//
//--


#ifndef HORTON_GBASIS_INTS_H
#define HORTON_GBASIS_INTS_H

#include "iter_pow.h"

double gpt_coeff(long k, long n0, long n1, double pa, double pb);
double gb_overlap_int1d(long n0, long n1, double pa, double pb, double gamma_inv);
const double gob_normalization(const double alpha, const long* n);

class GB2Integral {
    private:
        void swap_work();
    protected:
        long nwork, max_shell_type, max_nbasis;
        long shell_type0, shell_type1;
        double *work_pure, *work_cart;
        const double *r0, *r1;
        IterPow2 i2p;
    public:
        GB2Integral(long max_shell_type);
        ~GB2Integral();
        const long get_max_shell_type() const {return max_shell_type;};
        const long get_max_nbasis() const {return max_nbasis;};
        void reset(long shell_type0, long shell_type1, const double* r0, const double* r1);
        virtual void add(double coeff, double alpha0, double alpha1, const double* scales0, const double* scales1) = 0;
        void cart_to_pure();

        const long get_shell_type0() const {return shell_type0;};
        const long get_shell_type1() const {return shell_type1;};
        const double* get_work() const {return work_cart;};
    };


class GB2OverlapIntegral: public GB2Integral {
    public:
        GB2OverlapIntegral(long max_shell_type) : GB2Integral(max_shell_type) {}
        virtual void add(double coeff, double alpha0, double alpha1, const double* scales0, const double* scales1);
    };


class GB2KineticIntegral: public GB2Integral {
    public:
        GB2KineticIntegral(long max_shell_type) : GB2Integral(max_shell_type) {}
        virtual void add(double coeff, double alpha0, double alpha1, const double* scales0, const double* scales1);
    };


void nuclear_attraction_helper(double* work_g, long n0, long n1, double pa, double pb, double cp, double gamma_inv);

class GB2NuclearAttractionIntegral: public GB2Integral {
    private:
        double* charges;
        double* centers;
        long ncharge;

        double* work_g0;
        double* work_g1;
        double* work_g2;
        double* work_boys;
    public:
        GB2NuclearAttractionIntegral(long max_shell_type, double* charges, double* centers, long ncharge);
        ~GB2NuclearAttractionIntegral();
        virtual void add(double coeff, double alpha0, double alpha1, const double* scales0, const double* scales1);
    };

#endif
