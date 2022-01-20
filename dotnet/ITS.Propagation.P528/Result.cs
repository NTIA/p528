using System.Runtime.InteropServices;

namespace ITS.Propagation
{
    /// <summary>
    /// Recommendation ITU-R P.528-5
    /// </summary>
    public static partial class P528
    {
        /// <summary>
        /// Mode of Propagation
        /// </summary>
        public enum ModeOfPropagation : int
        {
            /// <summary>
            /// Value not set
            /// </summary>
            NotSet = 0,

            /// <summary>
            /// Line of Sight
            /// </summary>
            LineOfSight = 1,

            /// <summary>
            /// Diffraction
            /// </summary>
            Diffraction = 2,

            /// <summary>
            /// Troposcatter
            /// </summary>
            Troposcatter = 3
        }

        /// <summary>
        /// Results Data Structure
        /// </summary>
        [StructLayout(LayoutKind.Sequential)]
        public struct Result
        {
            /// <summary>
            /// Mode of propagation
            /// </summary>
            public ModeOfPropagation ModeOfPropagation;

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

            /// <summary>
            /// Warning flags
            /// </summary>
            public int warnings;
        }
    }
}
