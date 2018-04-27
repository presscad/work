﻿using System;
using System.Runtime.InteropServices;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WOTEDUtils
{
    [Guid("AA3F3188-2D24-4585-8044-AC2E1B6AE45A")]
    public interface EncInterface
    {
        [DispId(1)]
        string strEnc(string data, string firstKey, string secondKey, string thirdKey);
        [DispId(2)]
        string strEncSign6(string data1, string data2, string data3, string data4, string data5, string data6, string firstKey, string secondKey, string thirdKey);
        [DispId(3)]
        string strEncSign3(string data1, string data2, string data3, string firstKey, string secondKey, string thirdKey);
        [DispId(4)]
        string strEncSign5(string data1, string data2, string data3, string data4, string data5, string firstKey, string secondKey, string thirdKey);
        [DispId(5)]
        string strEncSign4(string data1, string data2, string data3, string data4, string firstKey, string secondKey, string thirdKey);
        [DispId(6)]
        string strEncSign9(string data1, string data2, string data3, string data4, string data5, string data6, string data7, string data8, string data9, string firstKey, string secondKey, string thirdKey);
        [DispId(7)]
        string strDec(string data, string firstKey, string secondKey, string thirdKey);
    }

    [Guid("937A91E8-3B05-4FB9-A6E5-BD2261533458"),
        ClassInterface(ClassInterfaceType.None)]
    public class DesUtils : EncInterface
    {
        public DesUtils()
        {
        }

        public string strEncSign6(string data1, string data2, string data3, string data4, string data5, string data6, string firstKey, string secondKey, string thirdKey)
        {
            string[] arr = { data1, data2, data3, data4, data5, data6 };
            return strEnc(naturalOrdering(arr), firstKey, secondKey, thirdKey);
        }
        public string strEncSign3(string data1, string data2, string data3, string firstKey, string secondKey, string thirdKey)
        {
            string[] arr = { data1, data2, data3 };
            return strEnc(naturalOrdering(arr), firstKey, secondKey, thirdKey);
        }
        public string strEncSign5(string data1, string data2, string data3, string data4, string data5, string firstKey, string secondKey, string thirdKey)
        {
            string[] arr = { data1, data2, data3, data4, data5 };
            return strEnc(naturalOrdering(arr), firstKey, secondKey, thirdKey);
        }
        public string strEncSign4(string data1, string data2, string data3, string data4, string firstKey, string secondKey, string thirdKey)
        {
            string[] arr = { data1, data2, data3, data4 };
            return strEnc(naturalOrdering(arr), firstKey, secondKey, thirdKey);
        }
        public string strEncSign9(string data1, string data2, string data3, string data4, string data5, string data6, string data7, string data8, string data9, string firstKey, string secondKey, string thirdKey)
        {
            string[] arr = { data1, data2, data3, data4, data5, data6, data7, data8, data9 };
            return strEnc(naturalOrdering(arr), firstKey, secondKey, thirdKey);
        }
        /// <summary>
        /// DES加密函数
        /// @Title: strEnc
        /// @Description: 加密函数 </summary>
        /// <param name="@param"> data </param>
        /// <param name="@param"> firstKey </param>
        /// <param name="@param"> secondKey </param>
        /// <param name="@param"> thirdKey </param>
        /// <param name="@return">DES算法加密后的字符串 </param>   
        /// @author 陈杨
        /// @date 2017年2月7日 下午13:48:12
        /// @version V1.0
        public string strEnc(string data, string firstKey, string secondKey, string thirdKey)
        {
            int leng = data.Length;
            string encData = "";
            IList firstKeyBt = null, secondKeyBt = null, thirdKeyBt = null;
            int firstLength = 0, secondLength = 0, thirdLength = 0;
            if (firstKey != null && firstKey != "")
            {
                firstKeyBt = getKeyBytes(firstKey);
                firstLength = firstKeyBt.Count;
            }
            if (secondKey != null && secondKey != "")
            {
                secondKeyBt = getKeyBytes(secondKey);
                secondLength = secondKeyBt.Count;
            }
            if (thirdKey != null && thirdKey != "")
            {
                thirdKeyBt = getKeyBytes(thirdKey);
                thirdLength = thirdKeyBt.Count;
            }
            if (leng > 0)
            {
                if (leng < 4)
                {
                    int[] bt = strToBt(data);
                    int[] encByte = null;
                    if (firstKey != null && firstKey != "" && secondKey != null
                        && secondKey != "" && thirdKey != null
                        && thirdKey != "")
                    {
                        int[] tempBt;
                        int x, y, z;
                        tempBt = bt;
                        for (x = 0; x < firstLength; x++)
                        {
                            tempBt = enc(tempBt, (int[])firstKeyBt[x]);
                        }
                        for (y = 0; y < secondLength; y++)
                        {
                            tempBt = enc(tempBt, (int[])secondKeyBt[y]);
                        }
                        for (z = 0; z < thirdLength; z++)
                        {
                            tempBt = enc(tempBt, (int[])thirdKeyBt[z]);
                        }
                        encByte = tempBt;
                    }
                    else
                    {
                        if (firstKey != null && firstKey != "" && secondKey != null
                            && secondKey != "")
                        {
                            int[] tempBt;
                            int x, y;
                            tempBt = bt;
                            for (x = 0; x < firstLength; x++)
                            {
                                tempBt = enc(tempBt, (int[])firstKeyBt[x]);
                            }
                            for (y = 0; y < secondLength; y++)
                            {
                                tempBt = enc(tempBt, (int[])secondKeyBt[y]);
                            }
                            encByte = tempBt;
                        }
                        else
                        {
                            if (firstKey != null && firstKey != "")
                            {
                                int[] tempBt;
                                int x = 0;
                                tempBt = bt;
                                for (x = 0; x < firstLength; x++)
                                {
                                    tempBt = enc(tempBt, (int[])firstKeyBt[x]);
                                }
                                encByte = tempBt;
                            }
                        }
                    }
                    encData = bt64ToHex(encByte);
                }
                else
                {
                    int iterator = (leng / 4);
                    int remainder = leng % 4;
                    int i = 0;
                    for (i = 0; i < iterator; i++)
                    {
                        string tempData = data.Substring(i * 4 + 0, (i * 4 + 4) - (i * 4 + 0));
                        int[] tempByte = strToBt(tempData);
                        int[] encByte = null;
                        if (firstKey != null && firstKey != "" && secondKey != null
                            && secondKey != "" && thirdKey != null
                            && thirdKey != "")
                        {
                            int[] tempBt;
                            int x, y, z;
                            tempBt = tempByte;
                            for (x = 0; x < firstLength; x++)
                            {
                                tempBt = enc(tempBt, (int[])firstKeyBt[x]);
                            }
                            for (y = 0; y < secondLength; y++)
                            {
                                tempBt = enc(tempBt, (int[])secondKeyBt[y]);
                            }
                            for (z = 0; z < thirdLength; z++)
                            {
                                tempBt = enc(tempBt, (int[])thirdKeyBt[z]);
                            }
                            encByte = tempBt;
                        }
                        else
                        {
                            if (firstKey != null && firstKey != ""
                                && secondKey != null && secondKey != "")
                            {
                                int[] tempBt;
                                int x, y;
                                tempBt = tempByte;
                                for (x = 0; x < firstLength; x++)
                                {
                                    tempBt = enc(tempBt, (int[])firstKeyBt[x]);
                                }
                                for (y = 0; y < secondLength; y++)
                                {
                                    tempBt = enc(tempBt, (int[])secondKeyBt[y]);
                                }
                                encByte = tempBt;
                            }
                            else
                            {
                                if (firstKey != null && firstKey != "")
                                {
                                    int[] tempBt;
                                    int x;
                                    tempBt = tempByte;
                                    for (x = 0; x < firstLength; x++)
                                    {
                                        tempBt = enc(tempBt, (int[])firstKeyBt[x]);
                                    }
                                    encByte = tempBt;
                                }
                            }
                        }
                        encData += bt64ToHex(encByte);
                    }
                    if (remainder > 0)
                    {
                        string remainderData = data.Substring(iterator * 4 + 0, leng - (iterator * 4 + 0));
                        int[] tempByte = strToBt(remainderData);
                        int[] encByte = null;
                        if (firstKey != null && firstKey != "" && secondKey != null
                            && secondKey != "" && thirdKey != null
                            && thirdKey != "")
                        {
                            int[] tempBt;
                            int x, y, z;
                            tempBt = tempByte;
                            for (x = 0; x < firstLength; x++)
                            {
                                tempBt = enc(tempBt, (int[])firstKeyBt[x]);
                            }
                            for (y = 0; y < secondLength; y++)
                            {
                                tempBt = enc(tempBt, (int[])secondKeyBt[y]);
                            }
                            for (z = 0; z < thirdLength; z++)
                            {
                                tempBt = enc(tempBt, (int[])thirdKeyBt[z]);
                            }
                            encByte = tempBt;
                        }
                        else
                        {
                            if (firstKey != null && firstKey != ""
                                && secondKey != null && secondKey != "")
                            {
                                int[] tempBt;
                                int x, y;
                                tempBt = tempByte;
                                for (x = 0; x < firstLength; x++)
                                {
                                    tempBt = enc(tempBt, (int[])firstKeyBt[x]);
                                }
                                for (y = 0; y < secondLength; y++)
                                {
                                    tempBt = enc(tempBt, (int[])secondKeyBt[y]);
                                }
                                encByte = tempBt;
                            }
                            else
                            {
                                if (firstKey != null && firstKey != "")
                                {
                                    int[] tempBt;
                                    int x;
                                    tempBt = tempByte;
                                    for (x = 0; x < firstLength; x++)
                                    {
                                        tempBt = enc(tempBt, (int[])firstKeyBt[x]);
                                    }
                                    encByte = tempBt;
                                }
                            }
                        }
                        encData += bt64ToHex(encByte);
                    }
                }
            }
            return encData;
        }
        /// <summary>
        /// DES解密函数
        /// @Title: strDec
        /// @Description: 解密函数 </summary>
        /// <param name="@param"> data </param>
        /// <param name="@param"> firstKey </param>
        /// <param name="@param"> secondKey </param>
        /// <param name="@param"> thirdKey </param>
        /// <param name="@return">DES算法解密后的字符串 </param>   
        /// @author 陈杨
        /// @date 2017年2月7日 下午14:02:19
        /// @version V1.0
		public string strDec(string data, string firstKey, string secondKey, string thirdKey)
        {
            int leng = data.Length;
            string decStr = "";
            IList firstKeyBt = null, secondKeyBt = null, thirdKeyBt = null;
            int firstLength = 0, secondLength = 0, thirdLength = 0;
            if (firstKey != null && firstKey != "")
            {
                firstKeyBt = getKeyBytes(firstKey);
                firstLength = firstKeyBt.Count;
            }
            if (secondKey != null && secondKey != "")
            {
                secondKeyBt = getKeyBytes(secondKey);
                secondLength = secondKeyBt.Count;
            }
            if (thirdKey != null && thirdKey != "")
            {
                thirdKeyBt = getKeyBytes(thirdKey);
                thirdLength = thirdKeyBt.Count;
            }
            int iterator = leng / 16;
            int i = 0;
            for (i = 0; i < iterator; i++)
            {
                string tempData = data.Substring(i * 16 + 0, (i * 16 + 16) - (i * 16 + 0));
                string strByte = hexToBt64(tempData);
                int[] intByte = new int[64];
                int j = 0;
                for (j = 0; j < 64; j++)
                {
                    intByte[j] = int.Parse(strByte.Substring(j, 1));
                }
                int[] decByte = null;
                if (firstKey != null && firstKey != "" && secondKey != null
                    && secondKey != "" && thirdKey != null && thirdKey != "")
                {
                    int[] tempBt;
                    int x, y, z;
                    tempBt = intByte;
                    for (x = thirdLength - 1; x >= 0; x--)
                    {
                        tempBt = dec(tempBt, (int[])thirdKeyBt[x]);
                    }
                    for (y = secondLength - 1; y >= 0; y--)
                    {
                        tempBt = dec(tempBt, (int[])secondKeyBt[y]);
                    }
                    for (z = firstLength - 1; z >= 0; z--)
                    {
                        tempBt = dec(tempBt, (int[])firstKeyBt[z]);
                    }
                    decByte = tempBt;
                }
                else
                {
                    if (firstKey != null && firstKey != "" && secondKey != null
                        && secondKey != "")
                    {
                        int[] tempBt;
                        int x, y;
                        tempBt = intByte;
                        for (x = secondLength - 1; x >= 0; x--)
                        {
                            tempBt = dec(tempBt, (int[])secondKeyBt[x]);
                        }
                        for (y = firstLength - 1; y >= 0; y--)
                        {
                            tempBt = dec(tempBt, (int[])firstKeyBt[y]);
                        }
                        decByte = tempBt;
                    }
                    else
                    {
                        if (firstKey != null && firstKey != "")
                        {
                            int[] tempBt;
                            int x;
                            tempBt = intByte;
                            for (x = firstLength - 1; x >= 0; x--)
                            {
                                tempBt = dec(tempBt, (int[])firstKeyBt[x]);
                            }
                            decByte = tempBt;
                        }
                    }
                }
                decStr += byteToString(decByte);
            }
            return decStr;
        }
        /// <summary>
        /// 将字符串转换成字节列表
        /// @Title: getKeyBytes
        /// @Description: 获取字节列表 </summary>
        /// <param name="@param"> key </param>
        /// <param name="@return"> </param>
        /// <returns> List    
        /// @exception:
        /// @author 陈杨
        /// @date 2017年2月7日 下午15:01:07
        /// @version V1.0
        public static IList getKeyBytes(string key)
        {
            IList keyBytes = new ArrayList();
            int leng = key.Length;
            int iterator = (leng / 4);
            int remainder = leng % 4;
            int i = 0;
            for (i = 0; i < iterator; i++)
            {
                keyBytes.Insert(i, strToBt(key.Substring(i * 4 + 0, (i * 4 + 4) - (i * 4 + 0))));
            }
            if (remainder > 0)
            {
                keyBytes.Insert(i, strToBt(key.Substring(i * 4 + 0, leng - (i * 4 + 0))));
            }
            return keyBytes;
        }
        /// <summary>
        /// 获取字节数组
        /// </summary>
        /// <param name="str"></param>
        /// <returns></returns>
        /// @author 陈杨
        /// @date 2017年2月7日 下午15:03:19
        /// @version V1.0
		public static int[] strToBt(string str)
        {
            int leng = str.Length;
            int[] bt = new int[64];
            if (leng < 4)
            {
                int i = 0, j = 0, p = 0, q = 0;
                for (i = 0; i < leng; i++)
                {
                    int k = str[i];
                    for (j = 0; j < 16; j++)
                    {
                        int pow = 1, m = 0;
                        for (m = 15; m > j; m--)
                        {
                            pow *= 2;
                        }
                        bt[16 * i + j] = (k / pow) % 2;
                    }
                }
                for (p = leng; p < 4; p++)
                {
                    int k = 0;
                    for (q = 0; q < 16; q++)
                    {
                        int pow = 1, m = 0;
                        for (m = 15; m > q; m--)
                        {
                            pow *= 2;
                        }
                        bt[16 * p + q] = (k / pow) % 2;
                    }
                }
            }
            else
            {
                for (int i = 0; i < 4; i++)
                {
                    int k = str[i];
                    for (int j = 0; j < 16; j++)
                    {
                        int pow = 1;
                        for (int m = 15; m > j; m--)
                        {
                            pow *= 2;
                        }
                        bt[16 * i + j] = (k / pow) % 2;
                    }
                }
            }
            return bt;
        }
        /// <summary>
        /// 获取十六进制字节数组
        /// </summary>
        /// <param name="binary"></param>
        /// <returns></returns>
        /// @author 陈杨
        /// @date 2017年2月7日 下午15:04:24
        /// @version V1.0
		public static string bt4ToHex(string binary)
        {
            string hex = "";
            if (binary.Equals("0000", StringComparison.CurrentCultureIgnoreCase))
            {
                hex = "0";
            }
            else if (binary.Equals("0001", StringComparison.CurrentCultureIgnoreCase))
            {
                hex = "1";
            }
            else if (binary.Equals("0010", StringComparison.CurrentCultureIgnoreCase))
            {
                hex = "2";
            }
            else if (binary.Equals("0011", StringComparison.CurrentCultureIgnoreCase))
            {
                hex = "3";
            }
            else if (binary.Equals("0100", StringComparison.CurrentCultureIgnoreCase))
            {
                hex = "4";
            }
            else if (binary.Equals("0101", StringComparison.CurrentCultureIgnoreCase))
            {
                hex = "5";
            }
            else if (binary.Equals("0110", StringComparison.CurrentCultureIgnoreCase))
            {
                hex = "6";
            }
            else if (binary.Equals("0111", StringComparison.CurrentCultureIgnoreCase))
            {
                hex = "7";
            }
            else if (binary.Equals("1000", StringComparison.CurrentCultureIgnoreCase))
            {
                hex = "8";
            }
            else if (binary.Equals("1001", StringComparison.CurrentCultureIgnoreCase))
            {
                hex = "9";
            }
            else if (binary.Equals("1010", StringComparison.CurrentCultureIgnoreCase))
            {
                hex = "A";
            }
            else if (binary.Equals("1011", StringComparison.CurrentCultureIgnoreCase))
            {
                hex = "B";
            }
            else if (binary.Equals("1100", StringComparison.CurrentCultureIgnoreCase))
            {
                hex = "C";
            }
            else if (binary.Equals("1101", StringComparison.CurrentCultureIgnoreCase))
            {
                hex = "D";
            }
            else if (binary.Equals("1110", StringComparison.CurrentCultureIgnoreCase))
            {
                hex = "E";
            }
            else if (binary.Equals("1111", StringComparison.CurrentCultureIgnoreCase))
            {
                hex = "F";
            }

            return hex;
        }
        /// <summary>
        /// 将十六进制数组转为二进制字节数组
        /// </summary>
        /// <param name="hex"></param>
        /// <returns></returns>
        /// @author 陈杨
        /// @date 2017年2月7日 下午15:07:02
        /// @version V1.0
		public static string hexToBt4(string hex)
        {
            string binary = "";
            if (hex.Equals("0", StringComparison.CurrentCultureIgnoreCase))
            {
                binary = "0000";
            }
            else if (hex.Equals("1", StringComparison.CurrentCultureIgnoreCase))
            {
                binary = "0001";
            }
            if (hex.Equals("2", StringComparison.CurrentCultureIgnoreCase))
            {
                binary = "0010";
            }
            if (hex.Equals("3", StringComparison.CurrentCultureIgnoreCase))
            {
                binary = "0011";
            }
            if (hex.Equals("4", StringComparison.CurrentCultureIgnoreCase))
            {
                binary = "0100";
            }
            if (hex.Equals("5", StringComparison.CurrentCultureIgnoreCase))
            {
                binary = "0101";
            }
            if (hex.Equals("6", StringComparison.CurrentCultureIgnoreCase))
            {
                binary = "0110";
            }
            if (hex.Equals("7", StringComparison.CurrentCultureIgnoreCase))
            {
                binary = "0111";
            }
            if (hex.Equals("8", StringComparison.CurrentCultureIgnoreCase))
            {
                binary = "1000";
            }
            if (hex.Equals("9", StringComparison.CurrentCultureIgnoreCase))
            {
                binary = "1001";
            }
            if (hex.Equals("A", StringComparison.CurrentCultureIgnoreCase))
            {
                binary = "1010";
            }
            if (hex.Equals("B", StringComparison.CurrentCultureIgnoreCase))
            {
                binary = "1011";
            }
            if (hex.Equals("C", StringComparison.CurrentCultureIgnoreCase))
            {
                binary = "1100";
            }
            if (hex.Equals("D", StringComparison.CurrentCultureIgnoreCase))
            {
                binary = "1101";
            }
            if (hex.Equals("E", StringComparison.CurrentCultureIgnoreCase))
            {
                binary = "1110";
            }
            if (hex.Equals("F", StringComparison.CurrentCultureIgnoreCase))
            {
                binary = "1111";
            }
            return binary;
        }
        /// <summary>
        /// 将二进制数组转为字符串
        /// </summary>
        /// <param name="byteData"></param>
        /// <returns></returns>
        /// @author 陈杨
        /// @date 2017年2月7日 下午15:17:18
        /// @version V1.0
		public static string byteToString(int[] byteData)
        {
            string str = "";
            for (int i = 0; i < 4; i++)
            {
                int count = 0;
                for (int j = 0; j < 16; j++)
                {
                    int pow = 1;
                    for (int m = 15; m > j; m--)
                    {
                        pow *= 2;
                    }
                    count += byteData[16 * i + j] * pow;
                }
                if (count != 0)
                {
                    str += "" + (char)(count);
                }
            }
            return str;
        }
        /// <summary>
        /// 将二进制数组转为十六进制数
        /// </summary>
        /// <param name="byteData"></param>
        /// <returns></returns>
        /// @author 陈杨
        /// @date 2017年2月7日 下午15:18:29
        /// @version V1.0
		public static string bt64ToHex(int[] byteData)
        {
            string hex = "";
            for (int i = 0; i < 16; i++)
            {
                string bt = "";
                for (int j = 0; j < 4; j++)
                {
                    bt += byteData[i * 4 + j];
                }
                hex += bt4ToHex(bt);
            }
            return hex;
        }
        /// <summary>
        /// 将字符串转为十六进制数
        /// </summary>
        /// <param name="hex"></param>
        /// <returns></returns>
        /// @author 陈杨
        /// @date 2017年2月7日 下午15:25:30
        /// @version V1.0
		public static string hexToBt64(string hex)
        {
            string binary = "";
            for (int i = 0; i < 16; i++)
            {
                binary += hexToBt4(hex.Substring(i, 1));
            }
            return binary;
        }
        /// <summary>
        /// 64位DES核心加密算法
        /// </summary>
        /// <param name="keyByte"></param>
        /// <param name="dataByte"></param>
        /// <returns></returns>
        /// @author 陈杨
        /// @date 2017年2月7日 下午15:30:12
        /// @version V1.0
		public static int[] enc(int[] dataByte, int[] keyByte)
        {
            int[][] keys = generateKeys(keyByte);
            int[] ipByte = initPermute(dataByte);
            int[] ipLeft = new int[32];
            int[] ipRight = new int[32];
            int[] tempLeft = new int[32];
            int i = 0, j = 0, k = 0, m = 0, n = 0;
            for (k = 0; k < 32; k++)
            {
                ipLeft[k] = ipByte[k];
                ipRight[k] = ipByte[32 + k];
            }
            for (i = 0; i < 16; i++)
            {
                for (j = 0; j < 32; j++)
                {
                    tempLeft[j] = ipLeft[j];
                    ipLeft[j] = ipRight[j];
                }
                int[] key = new int[48];
                for (m = 0; m < 48; m++)
                {
                    key[m] = keys[i][m];
                }
                int[] tempRight = xor(pPermute(sBoxPermute(xor(expandPermute(ipRight), key))), tempLeft);
                for (n = 0; n < 32; n++)
                {
                    ipRight[n] = tempRight[n];
                }

            }
            int[] finalData = new int[64];
            for (i = 0; i < 32; i++)
            {
                finalData[i] = ipRight[i];
                finalData[32 + i] = ipLeft[i];
            }
            return finallyPermute(finalData);
        }
        /// <summary>
        /// 64位DES核心解密算法
        /// </summary>
        /// <param name="keyByte"></param>
        /// <param name="dataByte"></param>
        /// <returns></returns>
        /// @author 陈杨
        /// @date 2017年2月7日 下午15:35:42
        /// @version V1.0
		public static int[] dec(int[] dataByte, int[] keyByte)
        {
            int[][] keys = generateKeys(keyByte);
            int[] ipByte = initPermute(dataByte);
            int[] ipLeft = new int[32];
            int[] ipRight = new int[32];
            int[] tempLeft = new int[32];
            int i = 0, j = 0, k = 0, m = 0, n = 0;
            for (k = 0; k < 32; k++)
            {
                ipLeft[k] = ipByte[k];
                ipRight[k] = ipByte[32 + k];
            }
            for (i = 15; i >= 0; i--)
            {
                for (j = 0; j < 32; j++)
                {
                    tempLeft[j] = ipLeft[j];
                    ipLeft[j] = ipRight[j];
                }
                int[] key = new int[48];
                for (m = 0; m < 48; m++)
                {
                    key[m] = keys[i][m];
                }

                int[] tempRight = xor(pPermute(sBoxPermute(xor(expandPermute(ipRight), key))), tempLeft);
                for (n = 0; n < 32; n++)
                {
                    ipRight[n] = tempRight[n];
                }
            }

            int[] finalData = new int[64];
            for (i = 0; i < 32; i++)
            {
                finalData[i] = ipRight[i];
                finalData[32 + i] = ipLeft[i];
            }
            return finallyPermute(finalData);
        }

        public static int[] initPermute(int[] originalData)
        {
            int[] ipByte = new int[64];
            int i = 0, m = 1, n = 0, j, k;
            for (i = 0, m = 1, n = 0; i < 4; i++, m += 2, n += 2)
            {
                for (j = 7, k = 0; j >= 0; j--, k++)
                {
                    ipByte[i * 8 + k] = originalData[j * 8 + m];
                    ipByte[i * 8 + k + 32] = originalData[j * 8 + n];
                }
            }
            return ipByte;
        }

        public static int[] expandPermute(int[] rightData)
        {
            int[] epByte = new int[48];
            int i;
            for (i = 0; i < 8; i++)
            {
                if (i == 0)
                {
                    epByte[i * 6 + 0] = rightData[31];
                }
                else
                {
                    epByte[i * 6 + 0] = rightData[i * 4 - 1];
                }
                epByte[i * 6 + 1] = rightData[i * 4 + 0];
                epByte[i * 6 + 2] = rightData[i * 4 + 1];
                epByte[i * 6 + 3] = rightData[i * 4 + 2];
                epByte[i * 6 + 4] = rightData[i * 4 + 3];
                if (i == 7)
                {
                    epByte[i * 6 + 5] = rightData[0];
                }
                else
                {
                    epByte[i * 6 + 5] = rightData[i * 4 + 4];
                }
            }
            return epByte;
        }

        public static int[] xor(int[] byteOne, int[] byteTwo)
        {
            int[] xorByte = new int[byteOne.Length];
            for (int i = 0; i < byteOne.Length; i++)
            {
                xorByte[i] = byteOne[i] ^ byteTwo[i];
            }
            return xorByte;
        }

        public static int[] sBoxPermute(int[] expandByte)
        {
            int[] sBoxByte = new int[32];
            string binary = "";
            int[][] s1 = new int[][]
            {
                new int[] {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
                new int[] {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
                new int[] {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
                new int[] {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}
            };
            int[][] s2 = new int[][]
            {
                new int[] {15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
                new int[] {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
                new int[] {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
                new int[] {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}
            };
            int[][] s3 = new int[][]
            {
                new int[] {10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
                new int[] {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
                new int[] {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
                new int[] {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}
            };
            int[][] s4 = new int[][]
            {
                new int[] {7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
                new int[] {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
                new int[] {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
                new int[] {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}
            };
            int[][] s5 = new int[][]
            {
                new int[] {2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
                new int[] {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
                new int[] {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
                new int[] {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}
            };
            int[][] s6 = new int[][]
            {
                new int[] {12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
                new int[] {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
                new int[] {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
                new int[] {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}
            };
            int[][] s7 = new int[][]
            {
                new int[] {4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
                new int[] {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
                new int[] {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
                new int[] {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}
            };
            int[][] s8 = new int[][]
            {
                new int[] {13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
                new int[] {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
                new int[] {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
                new int[] {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}
            };
            for (int m = 0; m < 8; m++)
            {
                int i = 0, j = 0;
                i = expandByte[m * 6 + 0] * 2 + expandByte[m * 6 + 5];
                j = expandByte[m * 6 + 1] * 2 * 2 * 2 + expandByte[m * 6 + 2] * 2 * 2 + expandByte[m * 6 + 3] * 2 + expandByte[m * 6 + 4];
                switch (m)
                {
                    case 0:
                        binary = getBoxBinary(s1[i][j]);
                        break;
                    case 1:
                        binary = getBoxBinary(s2[i][j]);
                        break;
                    case 2:
                        binary = getBoxBinary(s3[i][j]);
                        break;
                    case 3:
                        binary = getBoxBinary(s4[i][j]);
                        break;
                    case 4:
                        binary = getBoxBinary(s5[i][j]);
                        break;
                    case 5:
                        binary = getBoxBinary(s6[i][j]);
                        break;
                    case 6:
                        binary = getBoxBinary(s7[i][j]);
                        break;
                    case 7:
                        binary = getBoxBinary(s8[i][j]);
                        break;
                }
                sBoxByte[m * 4 + 0] = int.Parse(binary.Substring(0, 1));
                sBoxByte[m * 4 + 1] = int.Parse(binary.Substring(1, 1));
                sBoxByte[m * 4 + 2] = int.Parse(binary.Substring(2, 1));
                sBoxByte[m * 4 + 3] = int.Parse(binary.Substring(3, 1));
            }
            return sBoxByte;
        }

        public static int[] pPermute(int[] sBoxByte)
        {
            int[] pBoxPermute = new int[32];
            pBoxPermute[0] = sBoxByte[15];
            pBoxPermute[1] = sBoxByte[6];
            pBoxPermute[2] = sBoxByte[19];
            pBoxPermute[3] = sBoxByte[20];
            pBoxPermute[4] = sBoxByte[28];
            pBoxPermute[5] = sBoxByte[11];
            pBoxPermute[6] = sBoxByte[27];
            pBoxPermute[7] = sBoxByte[16];
            pBoxPermute[8] = sBoxByte[0];
            pBoxPermute[9] = sBoxByte[14];
            pBoxPermute[10] = sBoxByte[22];
            pBoxPermute[11] = sBoxByte[25];
            pBoxPermute[12] = sBoxByte[4];
            pBoxPermute[13] = sBoxByte[17];
            pBoxPermute[14] = sBoxByte[30];
            pBoxPermute[15] = sBoxByte[9];
            pBoxPermute[16] = sBoxByte[1];
            pBoxPermute[17] = sBoxByte[7];
            pBoxPermute[18] = sBoxByte[23];
            pBoxPermute[19] = sBoxByte[13];
            pBoxPermute[20] = sBoxByte[31];
            pBoxPermute[21] = sBoxByte[26];
            pBoxPermute[22] = sBoxByte[2];
            pBoxPermute[23] = sBoxByte[8];
            pBoxPermute[24] = sBoxByte[18];
            pBoxPermute[25] = sBoxByte[12];
            pBoxPermute[26] = sBoxByte[29];
            pBoxPermute[27] = sBoxByte[5];
            pBoxPermute[28] = sBoxByte[21];
            pBoxPermute[29] = sBoxByte[10];
            pBoxPermute[30] = sBoxByte[3];
            pBoxPermute[31] = sBoxByte[24];
            return pBoxPermute;
        }
        public static int[] finallyPermute(int[] endByte)
        {
            int[] fpByte = new int[64];
            fpByte[0] = endByte[39];
            fpByte[1] = endByte[7];
            fpByte[2] = endByte[47];
            fpByte[3] = endByte[15];
            fpByte[4] = endByte[55];
            fpByte[5] = endByte[23];
            fpByte[6] = endByte[63];
            fpByte[7] = endByte[31];
            fpByte[8] = endByte[38];
            fpByte[9] = endByte[6];
            fpByte[10] = endByte[46];
            fpByte[11] = endByte[14];
            fpByte[12] = endByte[54];
            fpByte[13] = endByte[22];
            fpByte[14] = endByte[62];
            fpByte[15] = endByte[30];
            fpByte[16] = endByte[37];
            fpByte[17] = endByte[5];
            fpByte[18] = endByte[45];
            fpByte[19] = endByte[13];
            fpByte[20] = endByte[53];
            fpByte[21] = endByte[21];
            fpByte[22] = endByte[61];
            fpByte[23] = endByte[29];
            fpByte[24] = endByte[36];
            fpByte[25] = endByte[4];
            fpByte[26] = endByte[44];
            fpByte[27] = endByte[12];
            fpByte[28] = endByte[52];
            fpByte[29] = endByte[20];
            fpByte[30] = endByte[60];
            fpByte[31] = endByte[28];
            fpByte[32] = endByte[35];
            fpByte[33] = endByte[3];
            fpByte[34] = endByte[43];
            fpByte[35] = endByte[11];
            fpByte[36] = endByte[51];
            fpByte[37] = endByte[19];
            fpByte[38] = endByte[59];
            fpByte[39] = endByte[27];
            fpByte[40] = endByte[34];
            fpByte[41] = endByte[2];
            fpByte[42] = endByte[42];
            fpByte[43] = endByte[10];
            fpByte[44] = endByte[50];
            fpByte[45] = endByte[18];
            fpByte[46] = endByte[58];
            fpByte[47] = endByte[26];
            fpByte[48] = endByte[33];
            fpByte[49] = endByte[1];
            fpByte[50] = endByte[41];
            fpByte[51] = endByte[9];
            fpByte[52] = endByte[49];
            fpByte[53] = endByte[17];
            fpByte[54] = endByte[57];
            fpByte[55] = endByte[25];
            fpByte[56] = endByte[32];
            fpByte[57] = endByte[0];
            fpByte[58] = endByte[40];
            fpByte[59] = endByte[8];
            fpByte[60] = endByte[48];
            fpByte[61] = endByte[16];
            fpByte[62] = endByte[56];
            fpByte[63] = endByte[24];
            return fpByte;
        }
        public static string getBoxBinary(int i)
        {
            string binary = "";
            switch (i)
            {
                case 0:
                    binary = "0000";
                    break;
                case 1:
                    binary = "0001";
                    break;
                case 2:
                    binary = "0010";
                    break;
                case 3:
                    binary = "0011";
                    break;
                case 4:
                    binary = "0100";
                    break;
                case 5:
                    binary = "0101";
                    break;
                case 6:
                    binary = "0110";
                    break;
                case 7:
                    binary = "0111";
                    break;
                case 8:
                    binary = "1000";
                    break;
                case 9:
                    binary = "1001";
                    break;
                case 10:
                    binary = "1010";
                    break;
                case 11:
                    binary = "1011";
                    break;
                case 12:
                    binary = "1100";
                    break;
                case 13:
                    binary = "1101";
                    break;
                case 14:
                    binary = "1110";
                    break;
                case 15:
                    binary = "1111";
                    break;
            }
            return binary;
        }
        public static int[][] ReturnRectangularIntArray(int size1, int size2)
        {
            int[][] newArray = new int[size1][];
            for (int array1 = 0; array1 < size1; array1++)
            {
                newArray[array1] = new int[size2];
            }

            return newArray;
        }
        public static int[][] generateKeys(int[] keyByte)
        {
            int[] key = new int[56];
            int[][] keys = ReturnRectangularIntArray(16, 48);
            int[] loop = new int[] { 1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1 };
            for (int i = 0; i < 7; i++)
            {
                for (int j = 0, k = 7; j < 8; j++, k--)
                {
                    key[i * 8 + j] = keyByte[8 * k + i];
                }
            }
            for (int i = 0; i < 16; i++)
            {
                int tempLeft = 0;
                int tempRight = 0;
                for (int j = 0; j < loop[i]; j++)
                {
                    tempLeft = key[0];
                    tempRight = key[28];
                    for (int k = 0; k < 27; k++)
                    {
                        key[k] = key[k + 1];
                        key[28 + k] = key[29 + k];
                    }
                    key[27] = tempLeft;
                    key[55] = tempRight;
                }
                int[] tempKey = new int[48];
                tempKey[0] = key[13];
                tempKey[1] = key[16];
                tempKey[2] = key[10];
                tempKey[3] = key[23];
                tempKey[4] = key[0];
                tempKey[5] = key[4];
                tempKey[6] = key[2];
                tempKey[7] = key[27];
                tempKey[8] = key[14];
                tempKey[9] = key[5];
                tempKey[10] = key[20];
                tempKey[11] = key[9];
                tempKey[12] = key[22];
                tempKey[13] = key[18];
                tempKey[14] = key[11];
                tempKey[15] = key[3];
                tempKey[16] = key[25];
                tempKey[17] = key[7];
                tempKey[18] = key[15];
                tempKey[19] = key[6];
                tempKey[20] = key[26];
                tempKey[21] = key[19];
                tempKey[22] = key[12];
                tempKey[23] = key[1];
                tempKey[24] = key[40];
                tempKey[25] = key[51];
                tempKey[26] = key[30];
                tempKey[27] = key[36];
                tempKey[28] = key[46];
                tempKey[29] = key[54];
                tempKey[30] = key[29];
                tempKey[31] = key[39];
                tempKey[32] = key[50];
                tempKey[33] = key[44];
                tempKey[34] = key[32];
                tempKey[35] = key[47];
                tempKey[36] = key[43];
                tempKey[37] = key[48];
                tempKey[38] = key[38];
                tempKey[39] = key[55];
                tempKey[40] = key[33];
                tempKey[41] = key[52];
                tempKey[42] = key[45];
                tempKey[43] = key[41];
                tempKey[44] = key[49];
                tempKey[45] = key[35];
                tempKey[46] = key[28];
                tempKey[47] = key[31];
                int m;
                switch (i)
                {
                    case 0:
                        for (m = 0; m < 48; m++)
                        {
                            keys[0][m] = tempKey[m];
                        }
                        break;
                    case 1:
                        for (m = 0; m < 48; m++)
                        {
                            keys[1][m] = tempKey[m];
                        }
                        break;
                    case 2:
                        for (m = 0; m < 48; m++)
                        {
                            keys[2][m] = tempKey[m];
                        }
                        break;
                    case 3:
                        for (m = 0; m < 48; m++)
                        {
                            keys[3][m] = tempKey[m];
                        }
                        break;
                    case 4:
                        for (m = 0; m < 48; m++)
                        {
                            keys[4][m] = tempKey[m];
                        }
                        break;
                    case 5:
                        for (m = 0; m < 48; m++)
                        {
                            keys[5][m] = tempKey[m];
                        }
                        break;
                    case 6:
                        for (m = 0; m < 48; m++)
                        {
                            keys[6][m] = tempKey[m];
                        }
                        break;
                    case 7:
                        for (m = 0; m < 48; m++)
                        {
                            keys[7][m] = tempKey[m];
                        }
                        break;
                    case 8:
                        for (m = 0; m < 48; m++)
                        {
                            keys[8][m] = tempKey[m];
                        }
                        break;
                    case 9:
                        for (m = 0; m < 48; m++)
                        {
                            keys[9][m] = tempKey[m];
                        }
                        break;
                    case 10:
                        for (m = 0; m < 48; m++)
                        {
                            keys[10][m] = tempKey[m];
                        }
                        break;
                    case 11:
                        for (m = 0; m < 48; m++)
                        {
                            keys[11][m] = tempKey[m];
                        }
                        break;
                    case 12:
                        for (m = 0; m < 48; m++)
                        {
                            keys[12][m] = tempKey[m];
                        }
                        break;
                    case 13:
                        for (m = 0; m < 48; m++)
                        {
                            keys[13][m] = tempKey[m];
                        }
                        break;
                    case 14:
                        for (m = 0; m < 48; m++)
                        {
                            keys[14][m] = tempKey[m];
                        }
                        break;
                    case 15:
                        for (m = 0; m < 48; m++)
                        {
                            keys[15][m] = tempKey[m];
                        }
                        break;
                }
            }
            return keys;
        }
        /// <summary>
        /// String[]数据各个元素的自然排序后拼接成一个用逗号分隔的字符串 </summary>
        /// <param name="strArray">
        /// @return </param>
        /// @author 陈杨
        /// @date 2017年8月2日 下午14:45:59
        /// @version V1.0
        public string naturalOrdering(string[] strArray)
        {
            string resultStr = "";

            if (strArray != null && strArray.Length > 0)
            {
                Array.Sort(strArray, string.CompareOrdinal);
                //C# 参数按照ASCII码从小到大排序（字典序） 
                /**for (int j = 1; j < strArray.Length; j++)
                {
                    for (int i = 0; i < strArray.Length - j; i++)
                    {
                        if (String.Compare(strArray[i], strArray[i + 1], StringComparison.Ordinal) > 0)
                        {
                            var temp = strArray[i];
                            strArray[i] = strArray[i + 1];

                            strArray[i + 1] = temp;
                        }
                    }
                }*/
                for (int i = 0; i < strArray.Length; i++)
                {
                    resultStr += (i == 0) ? strArray[i] : "," + strArray[i];
                }
            }
            return resultStr;
        }
    }
}