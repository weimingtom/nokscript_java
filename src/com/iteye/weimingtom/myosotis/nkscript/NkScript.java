package com.iteye.weimingtom.myosotis.nkscript;

import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.Reader;
import java.util.ArrayList;
import java.util.List;

public class NkScript {
	private List<ClassStruct> pCDefine = new ArrayList<ClassStruct>();
	private List<FuncStruct> pFunc = new ArrayList<FuncStruct>();
	
	public NkScript(String filename) throws Exception {
		//int size = GetFileSize(filename);
		String buffer = null;
		//buffer[size] = 0;
		
		StringBuffer sb = new StringBuffer();
		InputStream istr = null;
		BufferedReader buf;
		Reader reader = null;
		try {
			istr = new FileInputStream(filename);
			reader = new InputStreamReader(istr, "utf-8");
			buf = new BufferedReader(reader);
			String line;
			while (null != (line = buf.readLine())) {
				sb.append(line);
				sb.append("\n");
			}
			buffer = sb.toString();
		} catch (IOException e) {
			e.printStackTrace();
		} finally {
			if (reader != null) {
				try {
					reader.close();
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
			if (istr != null) {
				try {
					istr.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}
		//ReadFile(buffer, size, filename);
		//printf("%s\n", buffer);
		if (buffer == null) {
			MBF_ERR(filename + "²»´æÔÚ¡£");
			throw new Exception();
		}
		TokenArray tArray = new TokenArray();
		tArray.CutToToken(buffer);

		//FIXME:
		tArray.allprintf();
		System.out.println("" + tArray.GetArray().size());
		for (int i = 0; i < tArray.GetArray().size() && i >= 0; ++i) {
			if ("class".equals(tArray.GetArray().get(i).GetStr())) {
				pCDefine.add(new ClassStruct(tArray, i + 1));
				i = tArray.GetCorrespondence(i + 2, "{", "}");
			} else if (Misc.IsVar(tArray.GetArray().get(i).GetStr())) {
				int kakko = tArray.GetCorrespondence(i + 1, "(", ")");
				if (kakko < 0) {
					MBF_ERR(tArray.GetArray().get(i).GetStr() + ": function not found");
					throw new Exception();
				}
				int tyukakko = tArray.GetCorrespondence(kakko + 1, "{", "}");
				if (tyukakko < 0) {
					MBF_ERR(tArray.GetArray().get(i).GetStr() + ":function {} not found.");
					throw new Exception();
				}
				pFunc.add(new FuncStruct(tArray, i));
				i = tyukakko;
			} else if (";".equals(tArray.GetArray().get(i).GetStr())) {
				MBF_ERR(tArray.GetArray().get(i).GetStr() + ": not recognized.");
				throw new Exception();
			}
		}
		//SAFE_DELETE_ARRAY(buffer);
	}
	
	public void destoy() {
		int i;
		for (i = 0; i < pCDefine.size(); ++i) {
			pCDefine.set(i, null);
		}
		for (i = 0; i < pFunc.size(); ++i) {
			pFunc.set(i, null);
		}
		/* FIXME: */
		/*
		g_image.DeleteAll();
		g_sound.DeleteAll();
		*/
	}
	
	public int SearchClass(String name) {
		for (int i = 0; i < pCDefine.size(); ++i) {
			if (pCDefine.get(i).name.equals(name)) {
				return i;
			}
		}
		return -1;
	}
	
	public int SearchFunc(String name) {
		for (int i = 0; i < pFunc.size(); ++i) {
			if (pFunc.get(i).name.equals(name)) {
				return i;
			}
		}
		return -1;
	}
	
	public List<ClassStruct> GetClass() {
		return pCDefine;
	}
		
	public List<FuncStruct> GetFunc() {
		return pFunc;
	}
	
	private static void MB_ERR(String str) {
		System.out.println(str);
	}
	
	private static void MBF_ERR(String str) {
		System.out.println(str);
	}
}
