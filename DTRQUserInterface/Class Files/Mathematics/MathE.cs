﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ADRCVisualization.Class_Files.Mathematics
{
    public class MathE
    {
        /// <summary>
        /// Constrains the output of the input value to a maximum and minimum value.
        /// </summary>
        /// <param name="value"></param>
        /// <param name="minimum"></param>
        /// <param name="maximum"></param>
        /// <returns></returns>
        public static double Constrain(double value, double minimum, double maximum)
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

        /// <summary>
        /// 
        /// </summary>
        /// <param name="degrees"></param>
        /// <returns></returns>
        public static double DegreesToRadians(double degrees)
        {
            return degrees / (180.0 / Math.PI);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="radians"></param>
        /// <returns></returns>
        public static double RadiansToDegrees(double radians)
        {
            return radians * (180.0 / Math.PI);
        }
        
        /// <summary>
        /// 
        /// </summary>
        /// <param name="degrees"></param>
        /// <returns></returns>
        public static Vector DegreesToRadians(Vector degrees)
        {
            return degrees / (180.0 / Math.PI);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="radians"></param>
        /// <returns></returns>
        public static Vector RadiansToDegrees(Vector radians)
        {
            return radians * (180.0 / Math.PI);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="value"></param>
        /// <returns></returns>
        public static string DoubleToCleanString(double value)
        {
            return String.Format("{0:0.00}", value).PadLeft(8);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="values"></param>
        public static void CleanPrint(params double[] values)
        {
            string fullString = "";

            for (int i = 0; i < values.Length; i++)
            {
                fullString += DoubleToCleanString(values[i]) + " ";
            }

            Console.WriteLine(fullString);
        }

        public static void Atan3(double y, double x, double previousAngle)
        {

        }
    }
}
