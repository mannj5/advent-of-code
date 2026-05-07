class Validator:
    def __init__(self, fname):
        self.file = open(fname, "r")
        self.count = 0
    
    def get_count(self):
        return self.count

    def check_line(self, nums):
        dir = 1
        diff = int(nums[0]) - int(nums[1])
        if diff < 0:
            dir = -1

        for i in range(len(nums) - 1):
            diff = int(nums[i]) - int(nums[i + 1])
            if not self.check_diff(diff, dir):
                return False

        return True
    
    def check_diff(self, diff, dir):
        if diff == 0:
            return False

        if (diff < 0 and dir == 1) or (diff > 0 and dir == -1):
            return False
        
        if abs(diff) > 3:
            return False
        
        return True

    def try_removals(self, nums):
        for i in range(len(nums)):
            new_nums = nums[:i] + nums[i+1:]
            if self.check_line(new_nums):
                return True

        return False

    def validate(self):
        while(True):
            line = self.file.readline()
            if line == "":
                break
            
            line = line[:-1:]
            nums = line.split(" ")
            if self.check_line(nums):
                self.count += 1
            elif self.try_removals(nums):
                self.count += 1

v = Validator("input")
v.validate()
print(v.get_count())