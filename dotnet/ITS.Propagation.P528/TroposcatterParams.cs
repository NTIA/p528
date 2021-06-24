using System.Runtime.InteropServices;

namespace ITS.Propagation
{
    /// <summary>
    /// Recommendation ITU-R P.528-5
    /// </summary>
    public static partial class P528
    {
        /// <summary>
        /// Intermediate values for troposcatter calculations
        /// </summary>
        [StructLayout(LayoutKind.Sequential)]
        public struct TroposcatterParams
        {
            /// <summary>
            /// Scattering distance, in km
            /// </summary>
            public double d_s__km;

            /// <summary>
            /// Half the scattering distance, in km
            /// </summary>
            public double d_z__km;

            /// <summary>
            /// Height of the common volume cross-over point, in km
            /// </summary>
            public double h_v__km;

            /// <summary>
            /// Scattering angle, in radians
            /// </summary>
            public double theta_s__rad;

            /// <summary>
            /// Cross-over angle, in radians
            /// </summary>
            public double theta_A__rad;

            /// <summary>
            /// Troposcatter Loss, in dB
            /// </summary>
            public double A_s__db;

            /// <summary>
            /// Troposcatter Loss of previous test point, in dB
            /// </summary>
            public double A_s_prev__db;

            /// <summary>
            /// Troposcatter line slope, in dB/km
            /// </summary>
            public double M_s;
        };
    }
}
