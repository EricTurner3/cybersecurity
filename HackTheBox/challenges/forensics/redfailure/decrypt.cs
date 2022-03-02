
using System.Security.Cryptography;
using System.Text;
using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
					
public class redfailure
{
    // this file was my walkthrough of decrypting the shellcode from the 9tVI0 endpoint
    // there is a key in the 4A7xH.ps1 file that is required
    // other pieces of the code were ripped from the user32.dll file
    // all files can be found and extracted from the capture.pcap file provided
    // You can also take the key, IV and data[16::] and paste it in CyberChef to disassemble / download the shellcode
	
	public static void Main()
	{
		string password = "z64&Rx27Z$B%73up"; // password from our ps1 file
		Console.WriteLine("Key:");
		byte[] key = SHA256.Create().ComputeHash(Encoding.UTF8.GetBytes(password)); // method used in AES.cs
		//Output the key to the console
        using (SHA256 sha256Hash = SHA256.Create())
        {
            string hash = GetHash(sha256Hash, password);
			Console.WriteLine(hash);
		}
		Console.WriteLine("\r\n");
		//from WireShark, RAW data of /9tVI0
		string data = "9907bb679e1765dcbdb467c1c4b00d213b3f708679dc12e5352ff4ac0fbbdf6a57e4fa094a4d03ffba9ef251c2c57100df04dff882dcd4373e0d0bba5c6b642c4e4d7e2e46bd25c20c5865c027fac0cad8a0120d3e5efd31c8f16fb87bf90718b91b47592fac8834dc1b1c92d1efa0087edd678746421c01d4d22aa3b600649daacd7f0d2f7e9a9c9057c13ea6798c158fd843de556542ac477f20f6386df535a5dd46199b168bb2b13dc32ee1c9d4b20147442d08dfd1941ae034b5ff76a89f01cdf16a35e257927caa02d2b654bb85de2757a0a42793721bbc257d90b757dd0847d331776bb6b96800168f12204938fbec003ce9ab5e90b5bc57b9ac79efc405163028bd0c494d47db4f973d43dd62df1eeb809105afff6d6e8a0ea653ec9c03a620954981f65bdb4714abbdcf1613fce7a844f0c794dd2ba1811435fa62eed2c3da753437bcaa4722739ec365e1d6";
		//the Decrypt function expects a byte array so we have to convert
		byte[] dataBytes = StringToByteArray(data);
		//now decrypt
		byte[] numArray = Decrypt(key, dataBytes);
		//convert to shell to output
		Console.WriteLine("Shellcode:");
		Console.WriteLine(BitConverter.ToString(numArray).Replace("-", string.Empty));
	}
	
	// https://docs.microsoft.com/en-us/dotnet/api/system.security.cryptography.hashalgorithm.computehash?view=net-6.0#system-security-cryptography-hashalgorithm-computehash(system-byte())
	private static string GetHash(HashAlgorithm hashAlgorithm, string input)
    {

        // Convert the input string to a byte array and compute the hash.
        byte[] data = hashAlgorithm.ComputeHash(Encoding.UTF8.GetBytes(input));

        // Create a new Stringbuilder to collect the bytes
        // and create a string.
        var sBuilder = new StringBuilder();

        // Loop through each byte of the hashed data
        // and format each one as a hexadecimal string.
        for (int i = 0; i < data.Length; i++)
        {
            sBuilder.Append(data[i].ToString("x2"));
        }

        // Return the hexadecimal string.
        return sBuilder.ToString();
    }
	
	//https://stackoverflow.com/questions/321370/how-can-i-convert-a-hex-string-to-a-byte-array
	public static byte[] StringToByteArray(string hex) {
    return Enumerable.Range(0, hex.Length)
                     .Where(x => x % 2 == 0)
                     .Select(x => Convert.ToByte(hex.Substring(x, 2), 16))
                     .ToArray();
	}
	
	// from the AES.cs file of user32.dll
	private static byte[] PerformCryptography(ICryptoTransform cryptoTransform, byte[] data)
    {
      using (MemoryStream memoryStream = new MemoryStream())
      {
        using (CryptoStream cryptoStream = new CryptoStream((Stream) memoryStream, cryptoTransform, CryptoStreamMode.Write))
        {
          cryptoStream.Write(data, 0, data.Length);
          cryptoStream.FlushFinalBlock();
          return memoryStream.ToArray();
        }
      }
    }
	
	// from the AES.cs file of user32.dll
	public static byte[] Decrypt(byte[] key, byte[] data)
    {
      using (AesCryptoServiceProvider cryptoServiceProvider = new AesCryptoServiceProvider())
      {
        byte[] array1 = ((IEnumerable<byte>) data).Take<byte>(16).ToArray<byte>();
        byte[] array2 = ((IEnumerable<byte>) data).Skip<byte>(16).Take<byte>(data.Length - 16).ToArray<byte>();
        cryptoServiceProvider.Key = key;
        cryptoServiceProvider.IV = array1;
			Console.WriteLine("IV: "); // added for console output
			Console.WriteLine(BitConverter.ToString(array1).Replace("-", string.Empty)); // added for console output
			Console.WriteLine("\r\n"); // added for console output
        cryptoServiceProvider.Mode = CipherMode.CBC;
        cryptoServiceProvider.Padding = PaddingMode.PKCS7;
        using (ICryptoTransform decryptor = cryptoServiceProvider.CreateDecryptor(cryptoServiceProvider.Key, cryptoServiceProvider.IV))
          return PerformCryptography(decryptor, array2);
      }
    }
}