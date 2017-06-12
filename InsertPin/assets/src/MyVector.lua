local MyVector = class("MyVector")
--创建一个MyVector
function MyVector:ctor()
	self._data = {}
end
--末尾插入一个数据
function MyVector:push_back(val)
	table.insert(self._data,table.getn(self._data) + 1,val)	
end
--获取指定位置的数据
function MyVector:at(i)
	return self._data[i]
end
--删除数据
function MyVector:erase(val)
	local i = 1 
    while self._data[i] do 
		if self._data[i] == val then 
			table.remove(self._data,i) 
		else 
			i = i + 1 
		end 
	end 
end
--查找数据,返回数据所在位置的下标
function MyVector:find(val)
	local i = 1 
    while self._data[i] do 
		if self._data[i] == val then 
		   return i;
		else 
			i = i + 1 
		end 
	end 
end
--获得Vector当前数据个数
function MyVector:size()
	return table.getn(self._data)
end
--清空Vector
function MyVector:clear()
	self._data = {};
end
return MyVector;