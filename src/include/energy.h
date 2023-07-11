#ifndef ENERGY_H
#define ENERGY_H

#include <ctime>
#include "DifferentialEvolution.h"
#include "environment.h"

class PotEnergy : public de::IOptimizable
{
public:
    double EvaluteCost(std::vector<double> inputs) const override
    {
        assert(inputs.size() == 2);

        double x = inputs[0];
        double y = inputs[1];

        return x * x + 2 * x * y + 3 * y * y;
    }

    unsigned int NumberOfParameters() const override
    {
        return 2;
    }

    std::vector<Constraints> GetConstraints() const override
    {
        std::vector<Constraints> constr(NumberOfParameters());
        for (auto& c : constr)
        {
            c = Constraints(-100.0, 100.0, true);
        }
        return constr;
    }
};


#endif