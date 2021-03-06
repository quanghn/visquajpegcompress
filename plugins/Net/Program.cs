﻿using System;
using System.Collections.Generic;
using System.Text;
using System.Collections;
using System.Collections.Specialized;
using System.Net;
using System.IO;
using System.Diagnostics;

namespace upload_test
{
    class Program
    {
        public static void UploadFilesToRemoteUrl(string url, string filename, string outfilename, string contenttype, NameValueCollection nvc)
        {
            System.Net.ServicePointManager.Expect100Continue = false;
            if ((contenttype == null) || (contenttype.Length == 0))
            {
                contenttype = "application/octet-stream";
            }
            
            string boundary = "----------------------------" + DateTime.Now.Ticks.ToString("x");

            WebRequest request = WebRequest.Create(url);
            request.ContentType = "multipart/form-data; boundary=" + boundary;
            request.Method = "POST";
            request.Credentials = System.Net.CredentialCache.DefaultCredentials;
            request.Proxy = null;

            Stream memStream = new System.IO.MemoryStream();
            byte[] boundarybytes = System.Text.Encoding.ASCII.GetBytes("\r\n--" + boundary + "\r\n");

            string formdataTemplate = "\r\n--" + boundary + "\r\nContent-Disposition: form-data; name=\"{0}\";\r\n\r\n{1}";

            foreach (string key in nvc.Keys)
            {
                string formitem = string.Format(formdataTemplate, key, nvc[key]);
                byte[] formitembytes = System.Text.Encoding.UTF8.GetBytes(formitem);
                memStream.Write(formitembytes, 0, formitembytes.Length);
            }

            memStream.Write(boundarybytes, 0, boundarybytes.Length);

            string headerTemplate = "Content-Disposition: form-data; name=\"{0}\"; filename=\"{1}\"\r\n Content-Type: " + contenttype + "\r\n\r\n";
            string header = string.Format(headerTemplate, "userfile", filename);
            byte[] headerbytes = System.Text.Encoding.UTF8.GetBytes(header);
            memStream.Write(headerbytes, 0, headerbytes.Length);

            FileStream fileStream = new FileStream(filename, FileMode.Open, FileAccess.Read);
            byte[] buffer = new byte[10240000];

            int bytesRead = 0;
            while ((bytesRead = fileStream.Read(buffer, 0, buffer.Length)) != 0)
            {
                memStream.Write(buffer, 0, bytesRead);
            }

            memStream.Write(boundarybytes, 0, boundarybytes.Length);
            fileStream.Close();

            request.ContentLength = memStream.Length;

            Stream requestStream = request.GetRequestStream();
            memStream.Position = 0;
            byte[] tempBuffer = new byte[memStream.Length];
            memStream.Read(tempBuffer, 0, tempBuffer.Length);
            memStream.Close();
            requestStream.Write(tempBuffer, 0, tempBuffer.Length);
            requestStream.Close();

            WebResponse response = request.GetResponse();
            Stream stream2 = response.GetResponseStream();
            BinaryReader binReader = new BinaryReader(stream2);
            FileStream outfile = File.Open(outfilename, FileMode.Create);
            BinaryWriter binWriter = new BinaryWriter(outfile);
            while ((bytesRead = binReader.Read(buffer, 0, buffer.Length)) != 0)
            {
                binWriter.Write(buffer, 0, bytesRead);
            }
            binReader.Close();
            binWriter.Close();
            outfile.Close();

            response.Close();
            request = null;
            response = null;
        }
        static void Main(string[] args)
        {
            NameValueCollection form = new NameValueCollection();
            form["token"] = "vietcv";
            UploadFilesToRemoteUrl(@"http://ws.lessbit.net/cgi-test.cgi", "test.jpg",  "out.jpg", "image/jpeg", form);
        }
    }
}
