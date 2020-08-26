using System;
using System.Runtime.InteropServices;

namespace ITS.Propagation
{
    /// <summary>
    /// Recommendation ITU-R P.528-4
    /// </summary>
    public static partial class P528
    {
        private const string ITM_x86_DLL_NAME = "p528_x86.dll";
        private const string ITM_x64_DLL_NAME = "p528_x64.dll";

        #region 32-Bit P/Invoke Definitions

        [DllImport(ITM_x86_DLL_NAME, CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi, EntryPoint = "P528")]
        private static extern int P528_x86(double d__km, double h_1__meter, double h_2__meter, double f__mhz, double time_percentage, ref Result result);

        [DllImport(ITM_x86_DLL_NAME, CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi, EntryPoint = "P528_Ex")]
        private static extern int P528Ex_x86(double d__km, double h_1__meter, double h_2__meter, double f__mhz, double time_percentage, ref Result result,
            ref Terminal terminal_1, ref Terminal terminal_2, ref TroposcatterParams tropo, ref Path path, ref LineOfSightParams los_params);

        #endregion

        #region 64-Bit P/Invoke Definitions

        [DllImport(ITM_x64_DLL_NAME, CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi, EntryPoint = "P528")]
        private static extern int P528_x64(double d__km, double h_1__meter, double h_2__meter, double f__mhz, double time_percentage, ref Result result);

        [DllImport(ITM_x64_DLL_NAME, CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi, EntryPoint = "P528_Ex")]
        private static extern int P528Ex_x64(double d__km, double h_1__meter, double h_2__meter, double f__mhz, double time_percentage, ref Result result,
            ref Terminal terminal_1, ref Terminal terminal_2, ref TroposcatterParams tropo, ref Path path, ref LineOfSightParams los_params);

        #endregion

        private delegate int P528Delegate(double d__km, double h_1__meter, double h_2__meter, double f__mhz, double time_percentage, ref Result result);
        private delegate int P528ExDelegate(double d__km, double h_1__meter, double h_2__meter, double f__mhz, double time_percentage, ref Result result,
            ref Terminal terminal_1, ref Terminal terminal_2, ref TroposcatterParams tropo, ref Path path, ref LineOfSightParams los_params);

        private static P528Delegate P528_Invoke;
        private static P528ExDelegate P528Ex_Invoke;

        static P528()
        {
            if (P528_Invoke == null)
            {
                // set the binding to the correct native DLL architecture

                if (Environment.Is64BitProcess)
                {
                    P528_Invoke = P528_x64;
                    P528Ex_Invoke = P528Ex_x64;
                }
                else
                {
                    P528_Invoke = P528_x86;
                    P528Ex_Invoke = P528Ex_x86;
                }
            }
        }

        /// <summary>
        /// Recommendation ITU-R P.528-4
        /// </summary>
        /// <param name="d__km">Path distance, in km</param>
        /// <param name="h_1__meter">Height of the low terminal, in meters</param>
        /// <param name="h_2__meter">Height of the high terminal, in meters</param>
        /// <param name="f__mhz">Frequency, in MHz</param>
        /// <param name="time_percentage">Time percentage (0 &lt; q &lt; 1)</param>
        /// <param name="result">Result data structure</param>
        /// <returns>Return code</returns>
        public static int Invoke(double d__km, double h_1__meter, double h_2__meter, double f__mhz, double time_percentage, out Result result)
        {
            result = new Result();

            return P528_Invoke(d__km, h_1__meter, h_2__meter, f__mhz, time_percentage, ref result);
        }

        /// <summary>
        /// Recommendation ITU-R P.528-4
        /// </summary>
        /// <param name="d__km">Path distance, in km</param>
        /// <param name="h_1__meter">Height of the low terminal, in meters</param>
        /// <param name="h_2__meter">Height of the high terminal, in meters</param>
        /// <param name="f__mhz">Frequency, in MHz</param>
        /// <param name="time_percentage">Time percentage (0 &lt; q &lt; 1)</param>
        /// <param name="result">Result data structure</param>
        /// <param name="terminal_1">Intermediate values for terminal 1 geometry</param>
        /// <param name="terminal_2">Intermediate values for terminal 2 geometry</param>
        /// <param name="tropo">Intermediate values for troposcatter calculations</param>
        /// <param name="path">Intermediate values for propagation path</param>
        /// <param name="los_params">Intermediate values for Line-of-Sight calculations</param>
        /// <returns>Return code</returns>
        public static int InvokeEx(double d__km, double h_1__meter, double h_2__meter, double f__mhz, double time_percentage, out Result result,
            out Terminal terminal_1, out Terminal terminal_2, out TroposcatterParams tropo, out Path path, out LineOfSightParams los_params)
        {
            result = new Result();
            terminal_1 = new Terminal();
            terminal_2 = new Terminal();
            tropo = new TroposcatterParams();
            path = new Path();
            los_params = new LineOfSightParams();

            return P528Ex_Invoke(d__km, h_1__meter, h_2__meter, f__mhz, time_percentage, ref result, ref terminal_1, ref terminal_2, ref tropo, ref path, ref los_params);
        }
    }
}
