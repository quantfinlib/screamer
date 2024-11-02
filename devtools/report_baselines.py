from colorama import Fore, Style, init
from devtools import load_screamer_module, get_module_classes, get_baselines

def report_screamer_classes():
    # Initialize colorama
    init(autoreset=True)
    
    module = load_screamer_module()
    classes = get_module_classes(module)
    
    good_count = 0
    bad_count = 0

    for class_name in classes:
        if class_name.startswith('_'):
            continue
        references = get_baselines(base_name=class_name)
        
        if references:
            print(f"{Fore.GREEN}OK {class_name:20s} -> {', '.join([e[0] for e in references])}")
            good_count += 1
        else:
            print(f"{Fore.RED}{Style.BRIGHT}!! {class_name:20s}    Missing Baselines!")
            bad_count += 1
    
    # Print overview of successes and errors
    print("\nSummary:")
    print(f"{Fore.GREEN}Total OK: {good_count}")
    if bad_count > 0:
        print(f"{Fore.RED}Total Missing Baselines: {bad_count}")
    else:
        print(f"{Fore.GREEN}Total Missing Baselines: {bad_count}")
        
if __name__ == "__main__":
    report_screamer_classes()
