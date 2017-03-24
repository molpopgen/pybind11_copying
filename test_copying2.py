import copying2

o=copying2.Object(4)

def work_on_subobj(s):
    print(s.x)

def work_on_obj(o):
    print(o.s.x)

copying2.doit(o,work_on_subobj,work_on_obj)
