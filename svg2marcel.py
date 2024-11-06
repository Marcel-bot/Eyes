import re

def parseNum(num):
    return int(float(num)) + 3

def parse_path_to_beziers(path_string):
    command_re = re.compile(r'([A-Za-z])')
    number_re = re.compile(r'-?[\d.]+')

    commands = command_re.split(path_string)
    commands = [cmd.strip() for cmd in commands if cmd.strip()]

    beziers = []
    current_position = None

    i = 0
    while i < len(commands):
        command = commands[i]

        if command == 'M':  
            args = list(map(parseNum, number_re.findall(commands[i + 1])))
            current_position = (args[0], args[1])
            i += 2

        elif command == 'C':  
            args = list(map(parseNum, number_re.findall(commands[i + 1])))
            for j in range(0, len(args), 6):
                if j + 5 < len(args):
                    control1 = (args[j], args[j + 1])
                    control2 = (args[j + 2], args[j + 3])
                    end_point = (args[j + 4], args[j + 5])
                    beziers.append((current_position, control1, control2, end_point))
                    current_position = end_point
            i += 2

        elif command == 'L':  
            args = list(map(parseNum, number_re.findall(commands[i + 1])))
            end_point = (parseNum(args[0]), parseNum(args[1]))

            
            control1 = (
                parseNum(current_position[0] + 0.33 * (end_point[0] - current_position[0])),
                parseNum(current_position[1] + 0.33 * (end_point[1] - current_position[1]))
            )
            control2 = (
                parseNum(current_position[0] + 0.66 * (end_point[0] - current_position[0])),
                parseNum(current_position[1] + 0.66 * (end_point[1] - current_position[1]))
            )

            
            beziers.append((current_position, control1, control2, end_point))
            current_position = end_point
            i += 2

        elif command == 'Z':  
            i += 1

        else:
            i += 1

    return beziers

beziers = parse_path_to_beziers(input())

offsetX = 20
offsetY = 20


print('D ' + ' '.join([f'{bezier[0][0] + offsetX} {bezier[0][1] + offsetY} {bezier[1][0] + offsetX} {bezier[1][1] + offsetY} {bezier[2][0] + offsetX} {bezier[2][1] + offsetY}' for bezier in beziers]) + f' {beziers[0][0][0] + offsetX} {beziers[0][0][1] + offsetY}')