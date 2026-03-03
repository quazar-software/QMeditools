using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.NetworkInformation;
using System.Runtime.InteropServices;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using static SamplesCSharp.QMTDicomTagEnumerator;

namespace SamplesCSharp
{
    public class QMTDicomTag
    {
        [DllImport("QMediToolsCore.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.StdCall)]
        internal static extern void QMTReleaseEnumerator(IntPtr h);

        [DllImport("QMediToolsCore.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.StdCall)]
        internal static extern IntPtr QMTGetNextObject(IntPtr hEnum, [Out] QMT_TAGINFO Info);

        [DllImport("QMediToolsCore.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.StdCall)]
        internal static extern Int32 QMTGetTagInfo(IntPtr hTag, [Out] QMT_TAGINFO Info);

        [DllImport("QMediToolsCore.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.StdCall)]
        internal static extern void QMTReleaseObject(IntPtr h);

        // Declares a class member for each structure element.
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
        public class QMT_TAGINFO
        {
            public UInt32 nGroup = 0;
            public UInt32 nElement = 0;
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 65)]
            public string szPrivateCreator = null;
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 2)]
            public string cVR = null;
            public UInt32 nNumberOfValues = 0;
            public UInt64 nDataLength = 0;
            public UInt64 nOffset = 0;
        }
        public int Group { get; set; }
        public int Element { get; set; }
        public string PrivateCreator { get; set; }
        public string VR { get; set; }
        public int CountValue
        {
            get
            {
                if (_hTag == IntPtr.Zero) return 0;
                QMT_TAGINFO info = new QMT_TAGINFO();
                if (QMTGetTagInfo(_hTag, info) == 0)
                {
                    // what to copy more?
                }
                return (int) info.nNumberOfValues;
            }
        }
        public UInt32 SizeInBytes
        {
            get
            {
                if (_hTag == IntPtr.Zero) return 0;
                QMT_TAGINFO info = new QMT_TAGINFO();
                if (QMTGetTagInfo(_hTag, info) == 0)
                {
                    // what to copy more?
                }
                return (UInt32)info.nDataLength;
            }
        }

        public QMTDicomTag()
        {

        }
        public QMTDicomTag(IntPtr hTag)
        {
            _hTag = hTag;
            QMT_TAGINFO info = new QMT_TAGINFO();
            if (QMTGetTagInfo(_hTag, info) == 0)
            {
                Element = (int) info.nElement;
                Group = (int) info.nGroup;
                PrivateCreator = info.szPrivateCreator;
                VR = info.cVR;
            }
        }
        // Public implementation of Dispose pattern callable by consumers.
        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        private void Dispose(bool disposing)
        {
            if (!_disposedValue)
            {
                if (disposing)
                {
                    // TODO: dispose managed state (managed objects)
                }
                // TODO: free unmanaged resources (unmanaged objects) and override finalizer
                // TODO: set large fields to null
                if (_hTag != IntPtr.Zero) QMTReleaseObject(_hTag);
                _hTag = IntPtr.Zero;
                _disposedValue = true;
            }
        }
        private IntPtr _hTag = IntPtr.Zero;
        bool _disposedValue = false;
    }
}
