using System.Runtime.InteropServices;

namespace ITS.Propagation
{
    /// <summary>
    /// Recommendation ITU-R P.528-5
    /// </summary>
    public static partial class P528
    {
        /// <summary>
        /// Intermediate values for Line-of-Sight calculations
        /// </summary>
        [StructLayout(LayoutKind.Sequential)]
        public struct LineOfSightParams
        {
            /// <summary>
            /// Internal LOS param.  See Recommendation text.
            /// </summary>
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 2)]
            public double[] z__km;

            /// <summary>
            /// Path distance between terminals, in km
            /// </summary>
            public double d__km;

            /// <summary>
            /// Direct ray length, in km
            /// </summary>
            public double r_0__km;

            /// <summary>
            /// Indirect ray length, in km
            /// </summary>
            public double r_12__km;

            /// <summary>
            /// Internal LOS param.  See Recommendation text.
            /// </summary>
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 2)]
            public double[] D__km;

            /// <summary>
            /// Internal LOS param.  See Recommendation text.
            /// </summary>
            public double theta_h1__rad;

            /// <summary>
            /// Internal LOS param.  See Recommendation text.
            /// </summary>
            public double theta_h2__rad;

            /// <summary>
            /// Internal LOS param.  See Recommendation text.
            /// </summary>
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 2)]
            public double[] theta;

            /// <summary>
            /// Adjusted earth radius, in km
            /// </summary>
            public double a_a__km;

            /// <summary>
            /// Ray length path difference, in km
            /// </summary>
            public double delta_r__km;

            /// <summary>
            /// Loss due to LOS path, in dB
            /// </summary>
            public double A_LOS__db;
        };
    }
}
