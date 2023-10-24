import numpy as np
import matplotlib.pyplot as plt

# Create a figure with subplots
fig, axes = plt.subplots(3, 2, figsize=(10, 8))

# Sample data for each subplot
err_prob = [0, 10, 20, 30, 40]
transfer_time = [8.463, 12.801, 13.738, 18.267, 21.365]
throughput = [7065.224609 , 4670.963379 , 4352.380371, 3273.279541, 2798.642578]
data_unit_size = [100, 250, 500, 750, 1000]
transfer_time_2 = [42.320, 22.654, 8.463, 7.066, 4.931]  #transfer time vs data unit size for err = 0%
throughput_2 = [1412.878052, 2639.401611, 7065.224609, 8462.072266, 12125.937500]  #throughput vs data unit size for err = 0%
transfer_time_3 = [51.210, 27.774, 18.533, 12.002, 7.135] #transfer time vs data unit size for err = 30%
throughput_3 = [1167.604004, 2152.840820, 3226.298828, 4981.91992, 8380.238281]  #throughput vs data unit size for err = 30%

# Plot data on each subplot
axes[0, 0].plot(err_prob, transfer_time)
axes[0, 0].set_title('Transfer Time Vs Err Prob')
axes[0, 0].set_xlabel('Err Prob (%)')
axes[0, 0].set_ylabel('Transfer Time (ms)')

axes[0, 1].plot(err_prob, throughput)
axes[0, 1].set_title('Transfer Time Vs Throughput')
axes[0, 1].set_xlabel('Err Prob (%)')
axes[0, 1].set_ylabel('Throughput (KB/s)')

axes[1, 0].plot(data_unit_size, transfer_time_2)
axes[1, 0].set_title('Transfer Time Vs Data Unit Size, 0% Error')
axes[1, 0].set_xlabel('Data Unit Size (B)')
axes[1, 0].set_ylabel('Transfer Time (ms)')

axes[1, 1].plot(data_unit_size, throughput_2)
axes[1, 1].set_title('Throughput Vs Data Unit Size, 0% Error')
axes[1, 1].set_xlabel('Data Unit Size (B)')
axes[1, 1].set_ylabel('Throughput (KB/s)')

axes[2, 0].plot(data_unit_size, transfer_time_3)
axes[2, 0].set_title('Transfer Time Vs Data Unit Size, 30% Error')
axes[2, 0].set_xlabel('Data Unit Size (B)')
axes[2, 0].set_ylabel('Transfer Time (ms)')

axes[2, 1].plot(data_unit_size, throughput_3)
axes[2, 1].set_title('Throughput Vs Data Unit Size, 30% Error')
axes[2, 1].set_xlabel('Data Unit Size (B)')
axes[2, 1].set_ylabel('Throughput (KB/s)')

# Add an overall title for the entire figure

# Adjust layout to prevent overlap
plt.tight_layout()

# Show the plot
plt.show()