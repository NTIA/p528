using System.Runtime.InteropServices;

namespace ITS.Propagation
{
    /// <summary>
    /// Recommendation ITU-R P.528-5
    /// </summary>
    public static partial class P528
    {
        /// <summary>
        /// Results Data Structure
        /// </summary>
        [StructLayout(LayoutKind.Sequential)]
        public struct Result
        {
            /// <summary>
            /// Mode of propagation
            /// </summary>
            public int propagation_mode;

            /// <summary>
            /// Path distance used in calculations, in km
            /// </summary>
            public double d__km;

            /// <summary>
            /// Basic transmission loss, in dB
            /// </summary>
            public double A__db;

            /// <summary>
            /// Free space basic transmission loss, in dB
            /// </summary>
            public double A_fs__db;

            /// <summary>
            /// Median atmospheric absorption loss, in dB
            /// </summary>
            public double A_a__db;

            /// <summary>
            /// Elevation angle of the ray at the low terminal, in rad
            /// </summary>
            public double theta_h1__rad;
        }
    }
}
