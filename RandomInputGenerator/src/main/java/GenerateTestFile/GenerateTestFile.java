package GenerateTestFile;

import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.Objects;
import java.util.Random;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;

import abianalyser.StructType;

public class GenerateTestFile {
	public String str="(";
	
	public Random rnd = new Random();
	//general json object
	public JSONObject obj = new JSONObject();
	
	//transactions json array
	public JSONArray transactions = new JSONArray();
	
	int idcounter = 0;
	
	@SuppressWarnings("unchecked")
	public void WriteJSONFile(JSONArray test,ArrayList<StructType> val,String CName,List<String> bytecodes)
	{
		Iterator<StructType> iter = val.iterator(); 
		
		JSONArray _bytearradd = new JSONArray();
		JSONArray _intarradd = new JSONArray();
		JSONArray _bytesarradd = new JSONArray();
		while(iter.hasNext())
		{
			StructType stt = iter.next();
	
			//View functions will not added to input type
			if(stt.getstateMut().size()>=1 && stt.getstateMut().get(0)!=null)
			{
				if(stt.getstateMut().get(0).equals("view"))
				{
					return;
				}
			}
			
			if(stt.getFunTypes().get(0).equals("event"))
			{
				return;
			}
			
			
			//Object for account details
			JSONObject accountsDetails = new JSONObject();
			accountsDetails.put("account{0}", "0xca35b7d915458ef540ade6068dfe2f44e8fa733c");
			accountsDetails.put("account{1}", "0x14723a09acff6d2a60dcdf7aa4aff308fddc160c");
			accountsDetails.put("account{2}", "0x4b0897b0513fdc7c541b6d9d7e929c4e5364d2db");
			accountsDetails.put("account{3}", "0x583031d1113ad414f02576bd6afabfb302140225");
			accountsDetails.put("account{4}", "0xdd870fa1b7c4700f2bd7f44238821c26f7392148");
			
			
			
			//accountsDetails added to general object
			obj.put("accounts", accountsDetails);
			
			//Object for link references
			JSONObject linkreferences = new JSONObject();
			
			//link references added to general object
			obj.put("linkReferences",linkreferences);
			
			//JsonArray for transactions
			//JSONArray transactions = new JSONArray();
			
			
			//JsonObject inside the transactions - it will be for each transaction
			JSONObject transactionObject = new JSONObject();

			//unixTime created for timestamp
			long unixTime = System.currentTimeMillis() / 1000L;
			
			transactionObject.put("id", idcounter);
			idcounter++;
			
			//time stamp value is added
			transactionObject.put("timestamp", Objects.toString(unixTime));

			
			//recordDetails object is created
			JSONObject recordDetails = new JSONObject();

			
			
			if(stt.getPayable().get(0)=="false")
			{
				//value added
				recordDetails.put("value", "0");
			}
			else
			{
				
				//value added
				recordDetails.put("value",rnd.nextInt(200000000) );
			}
			
			//parameter array created
			JSONArray parameters = new JSONArray();
			//More than one parameters
			for(int k=0;k<stt.getinpValues().size();k++)
			{
				if(stt.getInpType().get(k).contains("bytes"))
				{
					for(int i =0;i<stt.bytestrarr.size();i++)
					{
						String prs2str = stt.bytestrarr.get(i);
						_bytesarradd.add(prs2str);
					}
					parameters.add(_bytesarradd);
					
				} else if(stt.getInpType().get(k).contains("byte"))
				{
					for(int i =0;i<stt.byteint.size();i++)
					{
						String prs2str = Integer.toString(stt.byteint.get(i));
						_bytearradd.add(prs2str);
					}
					parameters.add(_bytearradd);
					
				} else if(stt.getInpType().get(k).contains("int")){
					
					if(stt.getInpType().get(k).contains("[")) {
						for(int i =0;i<stt.intint.size();i++)
						{
							String prs2str = Integer.toString(stt.intint.get(i));
							_intarradd.add(prs2str);
						}
						parameters.add(_intarradd);

					}
					else {
						parameters.add(stt.getinpValues().get(k));
					}
					
				} 
				else
				{
					parameters.add(stt.getinpValues().get(k));
				}
				
				
			}

				
			//parameter object added to recordDetails
			recordDetails.put("parameters", parameters);
			
			//abi added to parameter details
			recordDetails.put("abi", "12345687954689451d564ad5132");
			
			
			//Special info for constructor type
			if(stt.funTypes.get(0).equals("constructor"))
			{
				//contractName added to parameter details
				recordDetails.put("contractName", CName);
					
				//byte-code added to recordDetails
				recordDetails.put("bytecode", bytecodes.get(0));
				//recordDetails.put("opcodes",opcodes.get(0));
				recordDetails.put("name","");
				//Link references object is created
				JSONObject linkreferencesinRecord = new JSONObject();
					
				//link references object added to RecordDetails
				recordDetails.put("linkReferences",linkreferencesinRecord);
			}
			else
			{
				recordDetails.put("to","created{"+ Objects.toString(unixTime)+"}");
				//name-type-from are added to recordDetails
				recordDetails.put("name",stt.getNames().get(0));
			}
			
			//More than one input type will be added according to format
			boolean inp=true;
			
			for(int r=0;r<stt.getInpType().size();r++)
			{
				if(r==stt.getInpType().size()-1)
					str += stt.getInpType().get(r)+")";
				else
					str +=stt.getInpType().get(r) + ",";
				
				recordDetails.put("inputs",str);
				inp=false;
			}
			if(inp)
			{
				recordDetails.put("inputs","()");
			}
			
			recordDetails.put("type", stt.getFunTypes().get(0));
			
			recordDetails.put("from", "account{"+new Random().nextInt(5)+"}");
			
			//recordDetails is added to TransactionObject
			transactionObject.put("record", recordDetails);
			

				transactions.add(transactionObject);
			//TransactionObject is added to Transactions Array
			
			str="(";
		}
		

		
		
	}
	@SuppressWarnings("unchecked")
	public void FinalWrite(JSONArray test,String output_file) {
		
		JSONObject abiobject = new JSONObject();
		abiobject.put("12345687954689451d564ad5132", test);
		
		
		//transactions array added to general object
		obj.put("transactions", transactions);
		obj.put("abis", abiobject);
		
		//Write JSON file
    	try (FileWriter file = new FileWriter(output_file)) {

            file.write(obj.toJSONString());
            file.flush();
            System.out.println("INPUT FILE IS CREATED");

        } catch (IOException e) {
            e.printStackTrace();
        }
	}

}
