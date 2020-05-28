using System.Runtime.InteropServices;

namespace ITS.Propagation
{
    /// <summary>
    /// Recommendation ITU-R P.528-4
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
            /// Path distance between terminals
            /// </summary>
            public double d__km;

            /// <summary>
            /// Direct ray length
            /// </summary>
            public double r_0__km;

            /// <summary>
            /// Indirect ray length
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
            public double theta_h1;

            /// <summary>
            /// Internal LOS param.  See Recommendation text.
            /// </summary>
            public double theta_h2;

            /// <summary>
            /// Internal LOS param.  See Recommendation text.
            /// </summary>
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 2)]
            public double[] theta;

            /// <summary>
            /// Adjusted earth radius
            /// </summary>
            public double a_a__km;

            /// <summary>
            /// Ray length path difference
            /// </summary>
            public double delta_r;

            /// <summary>
            /// Loss due to LOS path
            /// </summary>
            public double A_LOS__db;
        };
    }
}
