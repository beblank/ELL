////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Project:  Embedded Machine Learning Library (EMLL)
//  File:     BinaryErrorAggregator.cpp (evaluators)
//  Authors:  Ofer Dekel
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "BinaryErrorAggregator.h"

namespace evaluators
{
    std::vector<double> BinaryErrorAggregator::Value::GetValues() const
    {
        double sumFalse = sumFalsePositives + sumFalseNegatives;
        double sumTrue = sumTruePositives + sumTrueNegatives;

        double errorRate = sumFalse == 0.0 ? 0.0 : sumFalse / (sumTrue + sumFalse);
        double precision = sumTruePositives == 0.0 ? 0.0 : sumTruePositives / (sumTruePositives + sumFalsePositives);
        double recall = sumTruePositives == 0.0 ? 0.0 : sumTruePositives / (sumTruePositives + sumFalseNegatives);
        double f1 = (precision + recall) == 0.0 ? 0.0 : 2 * (precision * recall) / (precision + recall);

        return {errorRate, precision, recall, f1};
    }

    void BinaryErrorAggregator::Update(double prediction, double label, double weight)
    {
        if (label > 0)
        {
            if (prediction > 0)
            {
                _value.sumTruePositives += weight;
            }
            else
            {
                _value.sumFalseNegatives += weight;
            }
        }
        else
        {
            if (prediction >= 0)
            {
                _value.sumFalsePositives += weight;
            }
            else
            {
                _value.sumTrueNegatives += weight;
            }
        }
    }

    BinaryErrorAggregator::Value BinaryErrorAggregator::GetAndReset()
    {
        Value newValue;
        std::swap(_value, newValue);
        return newValue;
    }

    std::vector<std::string> BinaryErrorAggregator::GetValueNames() const 
    { 
        return {"ErrorRate", "Precision", "Recall", "F1-Score"}; 
    }
}