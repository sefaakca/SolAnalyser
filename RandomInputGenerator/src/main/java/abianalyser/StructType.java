package abianalyser;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public final class StructType {
	
	public String bytearr;
	
	public List<String> getNames() {
		return names;
	}
	public List<String> getInpType() {
		return inpType;
	}
	public List<String> getFunTypes() {
		return funTypes;
	}
	public List<String> getinpValues() {
		return inpValues;
	}
	public List<String> getstateMut() {
		return stateMut;
	}
	public List<String> getPayable(){
		return payable;
	}
	
	public List<String> names = new ArrayList<String>();
	public List<String> inpType = new ArrayList<String>();
	public List<String> funTypes = new ArrayList<String>();
	public List<String> inpValues = new ArrayList<String>();
	public List<String> stateMut = new ArrayList<String>();
	public List<Integer> byteint = new ArrayList<Integer>();
	public List<Integer> intint = new ArrayList<Integer>();
	public List<String> bytestrarr = new ArrayList<String>();
	public List<String> payable = new ArrayList<String>();
	
	public void addName(String name)
	{
		this.names.add(name);
	}
	public void addInpType(String type)
	{
		this.inpType.add(type);
	}
	public void addFunType(String fType)
	{
		this.funTypes.add(fType);
	}
	public void addInpValues(String inpVal)
	{
		this.inpValues.add(inpVal);
	}
	public void addInpValues2(String inpArr,String type) 
	{
		String newinpArr=inpArr.substring(1, inpArr.length()-1);
		String[] strArr=newinpArr.split(",");
		if(type.contains("bytes")) {
			for(int i=0;i<strArr.length;i++){
				bytestrarr.add(strArr[i].trim());
			}
		}else if(type.contains("byte"))
		{
			for(int i=0;i<strArr.length;i++){
				byteint.add(Integer.parseInt(strArr[i].trim()));
			}
		} else if (type.contains("int")) {
			for(int i=0;i<strArr.length;i++){
				intint.add(Integer.parseInt(strArr[i].trim()));
			}
		}/*else if(type.contains("address")||type.contains("Address")) {
			for(int i=0;i<strArr.length;i++){
				adrarr.add(strArr[i].trim());
			}
		}*/
		
		 bytearr= Arrays.deepToString(strArr);
		
		this.inpValues.add(bytearr);
		
	}
	public void addStateMu(String stateMu)
	{
		this.stateMut.add(stateMu);
	}
	public void addPayable(String _ispayable)
	{
		this.payable.add(_ispayable);
	}
	public void clear() {
		this.names.clear();
		this.funTypes.clear();
		this.inpType.clear();
		this.inpValues.clear();
		this.stateMut.clear();
		this.byteint.clear();
		this.intint.clear();
		this.payable.clear();
		this.bytestrarr.clear();
	}
	
	

}
