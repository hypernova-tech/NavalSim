public class CObjectContainer
{
    Dictionary<string, CObjectInfo> Objects = new Dictionary<string, CObjectInfo>();

    public bool Contains(string name)
    {
        if(Objects.TryGetValue(name, out var model))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    public void Add(string name, CObjectInfo model)
    {
        if(!Contains(name))
        {
            Objects.Add(name, model);
        }
        
    }
    public List<string> GetObjects()
    {
        var models =  Objects.Keys.ToList();
        return models;
    }

    public List<string> GetCategoriesForObject(string name)
    {
        List<string> ret = new List<string>();

        foreach (var pair in Objects)
        {
            if(pair.Key == name)
            {
                ret.Add(pair.Value.MainCategory);
            }
        }

        return ret;
    }
    public List<string> GetObjectsForCategories(string category)
    {
        List<string> ret = new List<string>();

        foreach (var pair in Objects)
        {
            if (pair.Value.MainCategory == category)
            {
                ret.Add(pair.Value.Name);
            }
        }

        return ret;
    }

    public HashSet<string> GetCategories()
    {
        HashSet<string> ret = new HashSet<string>();

        foreach (var pair in Objects)
        {
            ret.Add(pair.Value.MainCategory); ;
        }

        return ret;
    }

    internal void Clear()
    {
        Objects.Clear();
    }
}
