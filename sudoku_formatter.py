import lldb

def to_string_summary_provider(valobj, dict):
    try:
        # Call the to_string() method of the object
        to_string_val = valobj.GetChildMemberWithName("to_string()")
        if to_string_val.IsValid():
            return to_string_val.GetSummary()

        stream = lldb.SBStream()
        valobj.GetExpressionPath(stream)
        full_name = stream.GetData()
        print(full_name)
        # Alternative if GetChildMemberWithName("to_string()") doesn't work:
        # You may use `valobj.GetFrame().EvaluateExpression` to manually call the method.
        to_string_expr = valobj.GetFrame().EvaluateExpression("{}.to_string()".format(full_name))
        if to_string_expr.IsValid() and to_string_expr.GetError().Success():
            if to_string_expr.GetSummary():
                return to_string_expr.GetSummary()
            else:
                return to_string_expr.GetValue()

    except Exception as e:
        return "Error displaying MyType to string: {}".format(str(e))

    return "Error displaying MyType to string"

def __lldb_init_module(debugger, internal_dict):
    print('Initializing bitset summary provider')
    debugger.HandleCommand(
        'type summary add -F sudoku_formatter.to_string_summary_provider value_set cell_set cell'
    )

