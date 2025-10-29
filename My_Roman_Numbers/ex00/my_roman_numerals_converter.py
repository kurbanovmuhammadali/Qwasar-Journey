def my_roman_numerals_converter(param_1):
    roman_nums = {
        1000:'M',900: 'CM',
        500: 'D',400: 'CD',
        100: 'C',90: 'XC',
        50: 'L',40: 'XL',
        10: 'X',9: 'IX',
        5: 'V',4: 'IV',1:'I'
    }
    roman_str =''
    for key in sorted(roman_nums.keys(),reverse=True):
        while param_1 >= key:
            roman_str += roman_nums[key]
            param_1 -= key
    return roman_str