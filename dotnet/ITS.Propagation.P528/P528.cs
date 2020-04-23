using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace ITS.Propagation
{
    /// <summary>
    /// Recommendation ITU-R P.528-4
    /// </summary>
    /// <see cref="https://github.com/NTIA/p528"/>
    /// <seealso cref="https://www.itu.int/rec/R-REC-P.528/en"/>
    public static class P528
    {
        #region 64-Bit P/Invoke Definitions

        [DllImport("p528_x64.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi, EntryPoint = "Main")]
        private static extern int P528_x64(double d__km, double h_1__meter, double h_2__meter, double f__mhz, double time_percentage, ref P528Result result);

        [DllImport("p528_x64.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi, EntryPoint = "MainEx")]
        private static extern int P528Ex_x64(double d__km, double h_1__meter, double h_2__meter, double f__mhz, double time_percentage, ref P528Result result,
            ref P528Terminal terminal_1, ref P528Terminal terminal_2, ref P528TroposcatterParams tropo, ref P528Path path, ref P528LineOfSightParams los_params);

        #endregion

        #region 32-Bit P/Invoke Definitions

        [DllImport("p528_x86.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi, EntryPoint = "Main")]
        private static extern int P528_x86(double d__km, double h_1__meter, double h_2__meter, double f__mhz, double time_percentage, ref P528Result result);

        [DllImport("p528_x86.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi, EntryPoint = "MainEx")]
        private static extern int P528Ex_x86(double d__km, double h_1__meter, double h_2__meter, double f__mhz, double time_percentage, ref P528Result result,
            ref P528Terminal terminal_1, ref P528Terminal terminal_2, ref P528TroposcatterParams tropo, ref P528Path path, ref P528LineOfSightParams los_params);

        #endregion

        private delegate int P528Delegate(double d__km, double h_1__meter, double h_2__meter, double f__mhz, double time_percentage, ref P528Result result);
        private delegate int P528ExDelegate(double d__km, double h_1__meter, double h_2__meter, double f__mhz, double time_percentage, ref P528Result result,
            ref P528Terminal terminal_1, ref P528Terminal terminal_2, ref P528TroposcatterParams tropo, ref P528Path path, ref P528LineOfSightParams los_params);

        private static P528Delegate P528Call;
        private static P528ExDelegate P528ExCall;

        static P528()
        {
            if (P528Call == null)
            {
                // set the binding to the correct native DLL architecture

                if (Environment.Is64BitProcess)
                {
                    P528Call = P528_x64;
                    P528ExCall = P528Ex_x64;
                }
                else
                {
                    P528Call = P528_x86;
                    P528ExCall = P528Ex_x86;
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
        /// <returns>P.528 return code</returns>
        public static int Invoke(double d__km, double h_1__meter, double h_2__meter, double f__mhz, double time_percentage, out P528Result result)
        {
            result = new P528Result();

            return P528Call(d__km, h_1__meter, h_2__meter, f__mhz, time_percentage, ref result);
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
        /// <returns>P.528 return code</returns>
        public static int InvokeEx(double d__km, double h_1__meter, double h_2__meter, double f__mhz, double time_percentage, out P528Result result,
            out P528Terminal terminal_1, out P528Terminal terminal_2, out P528TroposcatterParams tropo, out P528Path path, out P528LineOfSightParams los_params)
        {
            result = new P528Result();
            terminal_1 = new P528Terminal();
            terminal_2 = new P528Terminal();
            tropo = new P528TroposcatterParams();
            path = new P528Path();
            los_params = new P528LineOfSightParams();

            return P528ExCall(d__km, h_1__meter, h_2__meter, f__mhz, time_percentage, ref result, ref terminal_1, ref terminal_2, ref tropo, ref path, ref los_params);
        }
    }
}
