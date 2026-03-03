using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics.Eventing.Reader;
using System.Linq;
using System.Net.Http.Headers;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace SamplesCSharp
{
    public class QMTDicomFile : IDisposable, IEnumerable<QMTDicomTag>
    {
        public enum QMT_XMLOUTPUTFORMAT { QMT_XMLFMT_Quazar=0, QMT_XMLFMT_dcm4che };
        
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
        public class QMT_INSTANCEINFO
        {
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 65)]
            public string szTransferSyntax = null;
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 65)]
            public string szSOPClassUID = null;
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 65)]
            public string szSOPInstanceUID = null;
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 65)]
            public string szRefFileID = null;
        };

        [DllImport("QMediToolsCore.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.StdCall)]
        internal static extern Int32 QMTGetOption(String sOptionName, StringBuilder sbReturn, Int32 sbReturnMaxCount);

        [DllImport("QMediToolsCore.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.StdCall)]
        internal static extern Int32 QMTSetOption(String sOptionName, String sValue);

        [DllImport("QMediToolsCore.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.StdCall)]
        internal static extern IntPtr QMTReadMessageFromFile(String sFilename, bool bParsePrivateTags);

        [DllImport("QMediToolsCore.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.StdCall)]
        internal static extern Int32 QMTGetInstanceInfoFromFile(String sFilename, [Out] QMT_INSTANCEINFO info);

        [DllImport("QMediToolsCore.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.StdCall)]
        internal static extern Int32 QMTFreeMessage(IntPtr h);

        [DllImport("QMediToolsCore.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.StdCall)]
        internal static extern Int32 QMTGetAttributeStr(IntPtr h, UInt16 nGroup, UInt16 nElement, StringBuilder sbReturn, Int32 sbReturnMaxCount);

        [DllImport("QMediToolsCore.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.StdCall)]
        internal static extern Int32 QMTSetAttributeStr(IntPtr h, UInt16 nGroup, UInt16 nElement, String sReturn);

        [DllImport("QMediToolsCore.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.StdCall)]
        internal static extern IntPtr QMTGetEnumerator(IntPtr h);

        [DllImport("QMediToolsCore.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.StdCall)]
        internal static extern void QMTReleaseEnumerator(IntPtr h);

        [DllImport("QMediToolsCore.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.StdCall)]
        internal static extern Int32 QMTWriteMessageToFile(IntPtr h, String sFilename, bool bAppendDicomFileHeader);

        [DllImport("QMediToolsCore.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.StdCall)]
        internal static extern Int32 QMTWriteMessageToXMLFile(IntPtr h, String sFilename, QMT_XMLOUTPUTFORMAT eFormat);

        [DllImport("QMediToolsCore.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.StdCall)]
        internal static extern Int32 QMTWriteDicomInstanceToFile(IntPtr h, String sFilename);

        [DllImport("QMediToolsCore.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.StdCall)]
        internal static extern Int32 QMTCreateInstanceUID(Int16 nSerialNo, StringBuilder sbUID, Int32 sbReturnMaxCount);

        public string Version
        {
            get
            {
                StringBuilder sb = new StringBuilder(255);
                if (QMTGetOption("version", sb, sb.Capacity) == 0x00) return sb.ToString();
                return "?";
            }
        }
        public string TransferSyntax { get; set; }
        public string SOPInstanceUID { get; set; }
        public string SOPClassUID { get; set; }

        public int NumberOfFrames
        {
            get
            {
                StringBuilder sb = new StringBuilder(255);
                if (_hMsg != IntPtr.Zero && QMTGetAttributeStr(_hMsg, 0x0028, 0x0008, sb, sb.Capacity) == 0x00) return sb.Length > 0 ? int.Parse(sb.ToString()) : 0;
                return 0;
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
                if (_hMsg != IntPtr.Zero) QMTFreeMessage(_hMsg);
                _hMsg = IntPtr.Zero;
                _disposedValue = true;
            }
        }

        public QMTDicomFile()
        {
            _hMsg = IntPtr.Zero;
        }
        ~QMTDicomFile() => Dispose(false);
        public void Load(string sFilename, bool bHeaderOnly=false)
        {
            if (_hMsg != IntPtr.Zero) QMTFreeMessage(_hMsg);
            if(bHeaderOnly)
            {
                QMT_INSTANCEINFO Info = new QMT_INSTANCEINFO();
                
                if(QMTGetInstanceInfoFromFile(sFilename, Info)==0)
                {
                    SOPClassUID = Info.szSOPClassUID;
                    SOPInstanceUID = Info.szSOPInstanceUID;
                    TransferSyntax = Info.szTransferSyntax;
                }
            }
            else
            {
                _hMsg = QMTReadMessageFromFile(sFilename, false);
                StringBuilder sb = new StringBuilder(255);
                if (_hMsg != IntPtr.Zero && QMTGetAttributeStr(_hMsg, 0x0002, 0x0010, sb, sb.Capacity) == 0x00)
                    TransferSyntax = sb.ToString();
                if (_hMsg != IntPtr.Zero && QMTGetAttributeStr(_hMsg, 0x0008, 0x0016, sb, sb.Capacity) == 0x00)
                    SOPClassUID = sb.ToString();
                if (_hMsg != IntPtr.Zero && QMTGetAttributeStr(_hMsg, 0x0008, 0x0018, sb, sb.Capacity) == 0x00)
                    SOPInstanceUID = sb.ToString();
            }
        }
        public static string CreateInstanceUID(Int16 nSerialNo=1)
        {
            StringBuilder sb = new StringBuilder(65);
            QMTCreateInstanceUID(nSerialNo,sb,sb.Capacity);
            return sb.ToString();
        }

        public void SaveXML(string sFilename)
        {
            if (_hMsg != IntPtr.Zero) QMTWriteMessageToXMLFile(_hMsg, sFilename, QMT_XMLOUTPUTFORMAT.QMT_XMLFMT_Quazar);
        }
        public void Save(string sFilename)
        {
            int hr = 0;
            if (_hMsg != IntPtr.Zero) hr=QMTWriteDicomInstanceToFile(_hMsg, sFilename);
        }
        public void SetUIDs(string sStudyInstanceUID, string sSeriesInstanceUID,string sInstanceUID)
        {
            int hr = 0;
            if (_hMsg != IntPtr.Zero)
            {
                hr = QMTSetAttributeStr(_hMsg,0x08, 0x18, sInstanceUID);
                hr = QMTSetAttributeStr(_hMsg, 0x20, 0x0d, sStudyInstanceUID);
                hr = QMTSetAttributeStr(_hMsg, 0x20, 0x0e, sSeriesInstanceUID);
            }
        }
        public void Close()
        {
            if (_hMsg != IntPtr.Zero) QMTFreeMessage(_hMsg);
            TransferSyntax = null;
            SOPClassUID = null;
            SOPInstanceUID = null;
        }

        public IEnumerator<QMTDicomTag> GetEnumerator()
        {
            IntPtr hEnum = QMTGetEnumerator(_hMsg);
            return new QMTDicomTagEnumerator(hEnum);
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            throw new NotImplementedException();
        }

        private IntPtr _hMsg = IntPtr.Zero;
        bool _disposedValue = false;
    }
}
