package abianalyser;

import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.lang.reflect.Array;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;
import java.util.logging.FileHandler;
import java.util.logging.Logger;
import java.util.logging.SimpleFormatter;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;

import GenerateTestFile.GenerateTestFile;
import randomGenerator.AdressGenerator;
import randomGenerator.BooleanGenerator;
import randomGenerator.ByteGenerator;
import randomGenerator.IntegerGenerator;
import randomGenerator.StringGenerator;

public class AnalyseAbiFile 
{
	public static IntegerGenerator intgen = new IntegerGenerator();
	public static StringGenerator strgen = new StringGenerator();
	public static ByteGenerator bytgen = new ByteGenerator();
	public static BooleanGenerator boolgen = new BooleanGenerator();
	public static AdressGenerator addgen = new AdressGenerator();
	public static GenerateTestFile _writeJson = new GenerateTestFile();
	
	public static JSONArray abiListRef= new JSONArray();
	
	public static ArrayList<StructType> inputList = new ArrayList<StructType>();
	public static StructType st = new StructType();
	
	 public static Logger logger = Logger.getLogger("MyLog");  
	 public static FileHandler fh;
	
	@SuppressWarnings("unchecked")
	public static void main(String[] args) throws SecurityException, IOException 
	{
		//JSON parser object to parse read file
		JSONParser jsonParser = new JSONParser();
		if(args.length<=3)
		{
			System.out.println("Please give your abi, ContractName, opcode and bytecode files as input!!");
			return;
		}
		String fileName = args[0];
		String contractName = args[1];
		//Path opcodesPath =Paths.get(args[2]);
		Path bytecodesPath = Paths.get(args[2]);
		String output_file = args[3];
		
		fh = new FileHandler("Random_Algorithm_Time.log",true);  
        logger.addHandler(fh);
        SimpleFormatter formatter = new SimpleFormatter();  
        fh.setFormatter(formatter);
        
        
		//timer started
		long startTime = System.currentTimeMillis();
		try (FileReader reader = new FileReader(fileName))
		{
			
			//List<String> opcodes = Files.readAllLines(opcodesPath);
			List<String> bytecodes = Files.readAllLines(bytecodesPath);

			
			
			//Read JSON file
            Object obj = jsonParser.parse(reader);

            JSONArray abiList = (JSONArray) obj;
            //Sort according to type
            List<JSONObject> jsonValues = new ArrayList<JSONObject>();
            for(int s=0; s<abiList.size();s++) {
            	jsonValues.add((JSONObject)abiList.get(s));
            }
            
            Collections.sort( jsonValues, new Comparator<JSONObject>() {
                private static final String KEY_NAME = "type";

                @Override
                public int compare(JSONObject a, JSONObject b) {
                    String valA = new String();
                    String valB = new String();

                    try {
                        valA = (String) a.get(KEY_NAME);
                        valB = (String) b.get(KEY_NAME);
                    } 
                    catch (Exception e) {
                       throw e;
                    }

                    return valA.compareTo(valB);
                }
            });
            for (int i = 0; i < abiList.size(); i++) {
            	abiListRef.add(jsonValues.get(i));
            }

            for(int test=0;test<100;test++) {
	            for(int i=0;i<abiListRef.size();i++)
	            {
	            	JSONObject teststst= (JSONObject)abiListRef.get(i);
	            	if(teststst.get("type").equals("constructor")&& test==0) {

		            	parseObjectInputs(((JSONObject)abiListRef.get(i)));
		            	parseFunctionType(((JSONObject)abiListRef.get(i)));
		            	parseFunctionName(((JSONObject)abiListRef.get(i)));
		            	parseStateMutability(((JSONObject)abiListRef.get(i)));
		            	parsePayableField(((JSONObject)abiListRef.get(i)));
		            	inputList.add(st);
						_writeJson.WriteJSONFile(abiListRef,inputList,contractName,bytecodes);
						st.clear();
						inputList.clear();
	            	} else if(!teststst.get("type").equals("constructor")) {
		            	parseObjectInputs(((JSONObject)abiListRef.get(i)));
		            	parseFunctionType(((JSONObject)abiListRef.get(i)));
		            	parseFunctionName(((JSONObject)abiListRef.get(i)));
		            	parseStateMutability(((JSONObject)abiListRef.get(i)));
		            	parsePayableField(((JSONObject)abiListRef.get(i)));
		            	inputList.add(st);
						_writeJson.WriteJSONFile(abiListRef,inputList,contractName,bytecodes);
						st.clear();
						inputList.clear();
	            	}
	            }
            }
          
            _writeJson.FinalWrite(abiListRef,output_file);
          //timer stopped
            long stopTime = System.currentTimeMillis();
            
            long elapsedTime = stopTime - startTime;
            System.out.println("TIME TAKEN: " + elapsedTime);
            logger.info(contractName + ": " + elapsedTime);
            System.out.println(contractName);
            
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        } catch (ParseException e) {
            e.printStackTrace();
        }
	}

	private static void parseObjectInputs(JSONObject inputs) 
	{
		try {
			//Get input lists
			JSONArray inputsObject = (JSONArray) inputs.get("inputs");
			Boolean btype=true;
			if(inputsObject!=null &&!inputsObject.isEmpty())
			{
				for(int i=0;i<inputsObject.size();i++)
				{
					//Get input Type
					JSONObject inputType = (JSONObject) inputsObject.get(i);
					btype=true;
					if(!inputType.isEmpty())
					{
						String vartype =inputType.get("type").toString();
						if(vartype.contains("int")) {
							if(vartype.contains("["))
							{
								int[] intret=intgen.GenerateIntegerArray(vartype);
								String _retval=Arrays.toString(intret);
								st.addInpType(vartype);
								
								st.addInpValues2(_retval,vartype);
							}
							else
							{
								String _retval=intgen.GenerateInteger(vartype);
								st.addInpType(vartype);
								st.addInpValues(_retval);
							}
						}
						else if(vartype.contains("String") || vartype.contains("string")) {
								String _retval=strgen.stringGenerator();
								st.addInpType(vartype);
								st.addInpValues(_retval);
							
						}
						else if(vartype.contains("byte")) {
							String[] intret=bytgen.byteGeneratorNewArray(vartype);
							String _retval=Arrays.toString(intret);
							st.addInpType(vartype);
							
							st.addInpValues2(_retval,vartype);
						}
						else if(vartype.equals("bool") || vartype.equals("Bool")) {
							String _retval=boolgen.boolGenerator();
							st.addInpType(vartype);
							st.addInpValues(_retval);

						}
						
						else if(vartype.contains("address") || vartype.contains("Address")) {
							
							if(!(vartype.contains("["))) {
								String add= addgen.addressGenerator();
								st.addInpType(vartype);
								st.addInpValues(add);
							}
							else {
								String[] add= addgen.addressGeneratorList();
								st.addInpType(vartype);
								st.addInpValues2(Arrays.deepToString(add),vartype);
							}
								
						}
						
					}
				}
			}
		}
		catch (Exception e) {
			throw e;
		}
		
	}
	
	private static void parseFunctionType(JSONObject f_Type)
	{
		try {
			String functionType = (String) f_Type.get("type");
			st.addFunType(functionType);

		}
		catch (Exception e) {
			throw e;
		}
		
	}
	
	private static void parseFunctionName(JSONObject f_Name)
	{
		try {
			String functionName = (String) f_Name.get("name");
			
			st.addName(functionName);
			
		} catch (Exception e) {
			// TODO: handle exception
		}
	}
	private static void parseStateMutability(JSONObject state)
	{
		try {
			String stateMutability = (String) state.get("stateMutability");

			st.addStateMu(stateMutability);


			
		} catch (Exception e) {
			// TODO: handle exception
		}
	}
	private static void parsePayableField(JSONObject state)
	{
		try {

			Boolean payableState = (Boolean) state.get("payable");
			
			st.addPayable(payableState.toString());

			
		} catch (Exception e) {
			// TODO: handle exception
		}
	}
}
