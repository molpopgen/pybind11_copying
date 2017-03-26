import copying3

o=copying3.Object(4)

def work_on_subobj(s):
    print(s.x)

def work_on_obj(o):
    print(o.s.x)

po = copying3.ProcessSubobject()

copying3.doit(o,po,work_on_obj)
