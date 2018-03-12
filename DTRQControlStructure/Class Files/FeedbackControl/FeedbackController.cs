﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ADRCVisualization.Class_Files.FeedbackControl
{
    public abstract class FeedbackController
    {
        public abstract double Calculate(double setpoint, double processVariable);
        public abstract double Calculate(double setpoint, double processVariable, double samplingPeriod);

        public abstract string SetOffset(double offset);
    }
}