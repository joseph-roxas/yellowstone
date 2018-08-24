﻿using System;
using System.IO;
using System.Xml;
using System.Xml.Serialization;

namespace WiFive
{
    public static class XmlConvert
    {
        public static string SerializeObject<T>(T dataObject)
        {
            if (dataObject == null)
            {
                return string.Empty;
            }
            try
            {
                using (StringWriter stringWriter = new StringWriter())
                {
                    var serializer = new XmlSerializer(typeof(T));
                    serializer.Serialize(stringWriter, dataObject);
                    return stringWriter.ToString();
                }
            }
            catch (Exception ex)
            {
                return string.Empty;
            }
        }

        public static T DeserializeObject<T>(string xml)
             where T : new()
        {
            if (string.IsNullOrEmpty(xml))
            {
                return new T();
            }
            try
            {
                using (var stringReader = new StringReader(xml))
                {
                    var serializer = new XmlSerializer(typeof(T));
                    var deserializedObject = (T)serializer.Deserialize(stringReader);
                    return deserializedObject;
                }
            }
            catch (Exception ex)
            {
                return new T();
            }
        }
    }
}
