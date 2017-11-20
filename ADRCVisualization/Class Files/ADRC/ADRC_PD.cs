﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ADRCVisualization.Class_Files
{
    class ADRC_PD
    {
        private PID pid;
        private ExtendedStateObserver ExtendedStateObserver;
        private NonlinearCombiner NonlinearCombiner;

        private DateTime dateTime;

        private double amplificationCoefficient;
        private double dampingCoefficient;
        private double precisionCoefficient;//0.2
        private double samplingPeriod;//0.05
        private double plantCoefficient;//b0 approximation
        private double precisionModifier;
        private double maxOutput;

        private double output;

        public ADRC_PD(double amplificationCoefficient, double dampingCoefficient, double plantCoefficient, double precisionModifier, double kp, double kd, double maxOutput)
        {
            this.amplificationCoefficient = amplificationCoefficient;
            this.dampingCoefficient = dampingCoefficient;
            this.plantCoefficient = plantCoefficient;
            this.precisionModifier = precisionModifier;
            this.maxOutput = maxOutput;

            pid = new PID(kp, 0, kd, maxOutput);
            ExtendedStateObserver = new ExtendedStateObserver(false);
            NonlinearCombiner = new NonlinearCombiner(amplificationCoefficient, dampingCoefficient);

            dateTime = DateTime.Now;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="setpoint"></param>
        /// <param name="pv"></param>
        /// <returns></returns>
        public double Calculate(double setpoint, double processVariable)
        {
            samplingPeriod = DateTime.Now.Subtract(dateTime).TotalSeconds;

            if (samplingPeriod > 0)
            {
                precisionCoefficient = samplingPeriod * precisionModifier;
                
                double temp = pid.Calculate(setpoint, processVariable, samplingPeriod);

                Tuple<double, double> test = new Tuple<double, double>(temp, temp);
                Tuple<double, double, double> eso = ExtendedStateObserver.ObserveState(samplingPeriod, output, plantCoefficient, processVariable);//double u, double y, double b0

                output = NonlinearCombiner.Combine(test, plantCoefficient, eso, precisionCoefficient);

                dateTime = DateTime.Now;
            }

            return Constrain(output, -maxOutput, maxOutput);
        }

        private double Constrain(double value, double minimum, double maximum)
        {
            if (value > maximum)
            {
                value = maximum;
            }
            else if (value < minimum)
            {
                value = minimum;
            }

            return value;
        }
    }
}